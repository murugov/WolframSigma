#include "./parser.hpp"
#include "OpInstrSet.cpp"
#include "wolfram.hpp"

#define PATH_TO_DATA "src/data.txt"                 // move to folder CONFIG

ht_t *variables = NULL;


lexerErr_t parserCtor(parser_t *parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) { return LEX_ERROR; } )

    FILE *SourceFile = fopen(PATH_TO_DATA, "r");
    if (IS_BAD_PTR(SourceFile)) { printf("Error: Cannot open file %s\n", PATH_TO_DATA); return LEX_ERROR; }
    
    char *buffer = NULL;
    int count_lines = 0;
    char **lines = DataReader(SourceFile, buffer, &count_lines);
    fclose(SourceFile);
    if (IS_BAD_PTR(lines)) { printf("Error: Failed to read file\n"); return LEX_ERROR; }
    
    lexer_t *lexer = (lexer_t*)calloc(1, sizeof(lexer_t));
    LexerCtor(lexer, lines, count_lines, PATH_TO_DATA);
    if (IS_BAD_PTR(lexer))
    {
        printf("Error: Failed to initialize lexer\n");
        FreeLines(lines, count_lines);
        return LEX_ERROR;
    }

    parser->lexer = lexer;

    stk_t<ht_t*>* name_tables = (stk_t<ht_t*>*)calloc(1, sizeof(stk_t<ht_t*>));
    if (IS_BAD_PTR(name_tables))
    { 
        LexerDtor(lexer);
        FreeLines(lines, count_lines);
        return LEX_ERROR; 
    }

    STACK_CTOR(name_tables, MIN_STK_LEN);
    
    ht_t* global_table = (ht_t*)calloc(1, sizeof(ht_t));
    if (global_table)
    {
        HT_CTOR(global_table);
        StackPush(name_tables, global_table);
    }

    variables = global_table;

    parser->name_tables    = name_tables;
    parser->cur_name_table = 0; 

    return LEX_SUCCESS;
}


lexerErr_t parserDtor(parser_t *parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) { return LEX_ERROR; } )

    if (!IS_BAD_PTR(parser->lexer))
    { 
        LexerDtor(parser->lexer);
        parser->lexer = NULL;
    }

    if (!IS_BAD_PTR(parser->name_tables))
    {
        for (ssize_t i = 0; i < parser->name_tables->size; ++i)
        {
            if (parser->name_tables->data[i])
            {
                HT_DTOR(parser->name_tables->data[i]);
            }
        }
        
        STACK_DTOR(parser->name_tables);
        free(parser->name_tables);
        parser->name_tables = NULL;
    }

    return LEX_SUCCESS;
}


int MatchToken(parser_t* parser, hash_t hash)
{
    if (CheckToken(parser, hash))
    { 
        CUR_POS++; 
        return 1; 
    }
    return 0;
}


int CheckToken(parser_t* parser, hash_t hash)
{
    if (parser->lexer->cur_token >= parser->lexer->tokens->size) { return 0; }
    
    return (CUR_HASH == hash);
}


token_t* ConsumeToken(parser_t* parser, hash_t hash, const char* error_msg)
{
    if (CheckToken(parser, hash))
    {
        token_t* token = CUR_TOKEN;
        CUR_POS++;
        return token;
    }

    PrintError(parser, CUR_TOKEN, error_msg);
    return NULL;
}


void PrintError(parser_t* parser, token_t* token, const char* message)
{
    ON_DEBUG( if (IS_BAD_PTR(parser) || IS_BAD_PTR(message)) return; )
    
    if (!IS_BAD_PTR(token))
    {
        printf(ANSI_COLOR_RED "Error at line %d, column %d: %s\n" ANSI_COLOR_RESET, token->line, token->col, message);
        printf(ANSI_COLOR_RED "Token: type=%d, hash=%zu\n" ANSI_COLOR_RESET, token->type, token->hash);
    }
    else 
    {
        printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, message);
    }
}


node_t* ParseAST(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) { return NULL; } )

    node_t* program    = NULL;
    node_t* expr       = NULL;
    node_t* last_expr  = NULL;
    
    while (!CheckToken(parser, HASH_EOF) && !CheckToken(parser, HASH_SEMICOLON))
    {
        expr = ParseExpression(parser);

        if (IS_BAD_PTR(expr)) { FreeNodes(program); return NULL; }

        if (IS_BAD_PTR(last_expr)) 
        { 
            program = expr; 
        }
        else 
        { 
            last_expr->right = expr; 
        }

        last_expr = expr;
    }

    set_parents(program, NULL);
    return program;
}


node_t* ParseExpression(parser_t* parser)
{
    node_t* node = ParseTerm(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(parser, HASH_ADD) || MatchToken(parser, HASH_SUB))
    {
        hash_t op_hash = PREV_HASH;
        
        node_t* right_node = ParseTerm(parser);
        if (IS_BAD_PTR(right_node)) { FreeNodes(node); return NULL; }
        
        node_t* op_node = OP_(op_hash);
        if (IS_BAD_PTR(op_node)) { FreeNodes(node); FreeNodes(right_node); return NULL; }
        
        op_node->left  = node;
        op_node->right = right_node;
        node           = op_node;
    }
    
    return node;
}


node_t* ParseTerm(parser_t* parser)
{
    node_t* node = ParseFactor(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(parser, HASH_MUL) || MatchToken(parser, HASH_DIV))
    {
        hash_t op_hash = PREV_HASH;
        
        node_t* right_node = ParseFactor(parser);
        if (IS_BAD_PTR(right_node)) { FreeNodes(node); return NULL; }
        
        node_t* op_node = OP_(op_hash);
        if (IS_BAD_PTR(op_node)) { FreeNodes(node); FreeNodes(right_node); return NULL; }
        
        op_node->left  = node;
        op_node->right = right_node;
        node           = op_node;
    }
    
    return node;
}


node_t* ParseFactor(parser_t* parser)
{
    node_t* node = ParsePrimary(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(parser, HASH_POW))
    {
        node_t* right_node = ParsePrimary(parser);
        if (IS_BAD_PTR(right_node)) { FreeNodes(node); return NULL; }
        
        node_t* op_node = OP_(HASH_POW);
        if (IS_BAD_PTR(op_node)) { FreeNodes(node); FreeNodes(right_node); return NULL; }
        
        op_node->left  = node;
        op_node->right = right_node;
        node           = op_node;
    }
    
    return node;
}


node_t* ParsePrimary(parser_t* parser)
{
    switch (CUR_TYPE)
    {
        case ARG_NUM:
            return ParseNum(parser);
        case ARG_VAR:
            return ParseVar(parser);
        case ARG_OP:
            if (!IS_BAD_PTR(FindOpByHash(CUR_HASH))) { return ParseFunc(parser); }
            else
            {
                PrintError(parser, CUR_TOKEN, "Unexpected operator");
                return NULL;
            }
        default:
            break;
    }

    if (MatchToken(parser, HASH_LPAREN))
    {
        node_t* expr = ParseExpression(parser);
        if (IS_BAD_PTR(expr)) { return NULL; }
        
        if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')' after expression")) { FreeNodes(expr); return NULL; }
        
        return expr;
    }
    
    PrintError(parser, CUR_TOKEN, "Expected number, variable, function, or '('");
    return NULL;
}


node_t* ParseFunc(parser_t* parser)
{    
    if (CUR_TYPE != ARG_OP)
    {
        if (CUR_TYPE == ARG_VAR) { return ParseVar(parser); }
        
        if (MatchToken(parser, HASH_LPAREN))
        {
            node_t* expr = ParseExpression(parser);
            if (IS_BAD_PTR(expr)) return NULL;
            
            if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')'")) { FreeNodes(expr); return NULL; }
            return expr;
        }
        
        PrintError(parser, CUR_TOKEN, "Expected function name or expression");
        return NULL;
    }
    
    token_t* func_token = CUR_TOKEN;
    hash_t func_hash = func_token->hash;
    
    op_t *builtin_func = FindOpByHash(CUR_HASH);
    int expected_args = builtin_func->num_args;
    
    MatchToken(parser, func_hash);
    
    if (!ConsumeToken(parser, HASH_LPAREN, "Expected '(' after function name")) { return NULL; }
    
    node_t* args = NULL;
    node_t* last_arg = NULL;
    int arg_count = 0;
    
    if (!CheckToken(parser, HASH_RPAREN))
    {
        do {
            node_t* arg = ParseExpression(parser);
            if (IS_BAD_PTR(arg)) { if (args) { FreeNodes(args); } return NULL; }  
            
            arg_count++;
            
            if (expected_args > 0 && arg_count > expected_args)
            {
                PrintError(parser, CUR_TOKEN, "Too many arguments");
                FreeNodes(arg);
                if (args) FreeNodes(args);
                return NULL;
            }
            
            if (IS_BAD_PTR(args))
            {
                args = arg;
                last_arg = arg;
            }
            else
            {
                last_arg->left = arg;
                last_arg = arg;
            }
        } while (MatchToken(parser, HASH_COMMA));
    }
    
    if (expected_args > 0 && arg_count < expected_args)
    {
        PrintError(parser, CUR_TOKEN, "Not enough arguments");
        if (args) { FreeNodes(args); }
        return NULL;
    }
    
    if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')' after arguments"))
    {
        if (args) { FreeNodes(args); }
        return NULL;
    }
    
    node_t* func_node = OP_(func_token->hash);
    if (IS_BAD_PTR(func_node)) { if (args) { FreeNodes(args); } return NULL; }
    
    if (arg_count == 1)
    {
        func_node->right = args;
    }
    else if (arg_count == 2)
    {
        node_t* first_arg = args;
        node_t* second_arg = NULL;
        
        if (first_arg && first_arg->left)
        {
            second_arg = first_arg->left;
            first_arg->left = NULL;
        }
        
        func_node->left = first_arg;
        func_node->right = second_arg;
        
        if (args && args != first_arg) FreeNodes(args);
    }
    else
    {
        func_node->left = args;
    }
    
    return func_node;
}


node_t* ParseVar(parser_t* parser)
{
    if (CUR_TYPE != ARG_VAR)
    {
        PrintError(parser, CUR_TOKEN, "Expected variable");
        return NULL;
    }
    
    node_t* var_node = VAR_(CUR_START, CUR_LEN);

    if (CUR_NAME_TABLE) { htInsert(CUR_NAME_TABLE, strndup(CUR_START, (size_t)CUR_LEN)); }

    MatchToken(parser, CUR_HASH);
    
    return var_node;
}


node_t* ParseNum(parser_t* parser)
{
    if (CUR_TYPE != ARG_NUM)
    {
        PrintError(parser, CUR_TOKEN, "Expected number");
        return NULL;
    }
    
    token_t* num_token = CUR_TOKEN;
    MatchToken(parser, CUR_HASH);
    
    double value = 0.0;                                             // переделать
    int num_digits = 0;
    sscanf(num_token->start, "%lg%n", &value, &num_digits);
    
    return NUM_(value);
}


op_t *FindOpByHash(hash_t hash)
{
    ON_DEBUG( if (IS_BAD_PTR(index)) { return NULL; } )

    return (op_t*)bsearch(&hash, op_instr_set, LEN_INSTR_SET, sizeof(op_instr_set[0]), CmpForOpSearch);
}


int CmpForOpSearch(const void *a, const void *b)
{
    const hash_t *hash_a = (const hash_t*)a;
    const op_t   *op_b   = (const op_t*)b;
    
    if (*hash_a > op_b->hash)
        return 1;
    if (*hash_a < op_b->hash)
        return -1;
    return 0;
}