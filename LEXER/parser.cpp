#include "parser.hpp"

#define PATH_TO_DATA "src/data.txt"                 // move to folder CONFIG


lexerErr_t parserCtor(parser_t *parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) { return LEX_ERROR; } )

    FILE *SourceFile = fopen(PATH_TO_DATA, "r");
    if (!SourceFile) { printf("Error: Cannot open file %s\n", PATH_TO_DATA); return LEX_ERROR; }
    
    char *buffer = NULL;
    int count_lines = 0;
    char **lines = DataReader(SourceFile, buffer, &count_lines);
    fclose(SourceFile);
    if (IS_BAD_PTR(lines)) { printf("Error: Failed to read file\n"); return LEX_ERROR; }
    
    lexer_t *lexer = (lexer_t*)calloc(1, sizeof(lexer_t));
    LexerCtor(lexer, lines, count_lines, PATH_TO_DATA);
    if (IS_BAD_PTR(lexer)) { printf("Error: Failed to initialize lexer\n"); FreeLines(lines, count_lines); return LEX_ERROR; }

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

    parser->name_tables    = name_tables;
    parser->cur_name_table = 0; 

    FreeLines(lines, count_lines);
    return LEX_SUCCESS;
}


lexerErr_t parserDtor(parser_t *parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) { return LEX_ERROR; } )

    if (parser->lexer) { LexerDtor(parser->lexer); parser->lexer = NULL; }

    if (parser->name_tables)
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
        parser->lexer->cur_token++; 
        return 1; 
    }
    return 0;
}


int CheckToken(parser_t* parser, hash_t hash)
{
    lexer_t* lexer = parser->lexer;
    if (lexer->cur_token >= lexer->tokens->size) { return 0; }
    
    token_t* token = lexer->tokens->data[lexer->cur_token];
    return (token->hash == hash);
}


token_t* ConsumeToken(parser_t* parser, hash_t hash, const char* error_msg)
{
    if (CheckToken(parser, hash))
    {
        token_t* token = parser->lexer->tokens->data[parser->lexer->cur_token];
        parser->lexer->cur_token++;
        return token;
    }

    PrintError(parser, CUR_TOKEN, error_msg);
    return NULL;
}


void PrintError(parser_t* parser, token_t* token, const char* message)
{
    ON_DEBUG( if (IS_BAD_PTR(parser) || IS_BAD_PTR(message)) return; )
    
    if (token)
    {
        printf("Error at line %d, column %d: %s\n", token->line, token->col, message);
        printf("Token: type=%d, hash=%lu\n", token->type, token->hash);
    }
    else 
    {
        printf("Error: %s\n", message);
    }
}


node_t* ParseAST(parser_t *parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) { return NULL; } )

    hash_t program_hash = 0;                                    // improve
    node_t* program = OP_(program_hash);
    if (IS_BAD_PTR(program)) return NULL;
    
    node_t* last_stmt = NULL;
    
    while (!MatchToken(parser, HASH_EOF))
    {
        node_t* stmt = NULL;
        
        if (CUR_TYPE == ARG_OP && CUR_HASH == HASH_DEF)
        {
            stmt = ParseFunc(parser);
        } 
        else if (CUR_TYPE == ARG_OP && CUR_HASH == HASH_IF)
        {
            stmt = ParseIf(parser);
        }
        else if (CUR_TYPE == ARG_OP && CUR_HASH == HASH_WHILE)
        {
            stmt = ParseWhile(parser);
        }
        else if (CUR_TYPE == ARG_OP && CUR_HASH == HASH_RETURN)
        {
            stmt = ParseReturn(parser);
        }
        else if (CUR_TYPE == ARG_VAR)
        {
            if (parser->lexer->cur_token + 1 < parser->lexer->tokens->size &&
                parser->lexer->tokens->data[parser->lexer->cur_token + 1]->hash == HASH_EQ)
            {
                stmt = ParseAssignment(parser);
            } 
            else
            {
                stmt = ParseExpression(parser);
                ConsumeToken(parser, HASH_SEMICOLON, "Expected ';' after expression");
            }
        }
        else
        {
            stmt = ParseExpression(parser);
            if (IS_BAD_PTR(stmt)) { ConsumeToken(parser, HASH_SEMICOLON, "Expected ';' after expression"); }
        }
        
        if (IS_BAD_PTR(stmt)) { free(program); return NULL; }
        
        if (IS_BAD_PTR(program->left))
        {
            program->left = stmt;
            last_stmt     = stmt;
        }
        else
        {
            last_stmt->right = stmt;
            last_stmt        = stmt;
        }
    }
    
    return program;
}


node_t* ParseFunc(parser_t* parser)
{
    MatchToken(parser, HASH_DEF);
    
    if (CUR_TYPE != ARG_FUNC && CUR_TYPE != ARG_VAR)
    {
        PrintError(parser, CUR_TOKEN, "Expected function name after 'def'");
        return NULL;
    }
    
    token_t* name_token = CUR_TOKEN;
    MatchToken(parser, name_token->hash);
    
    if (!ConsumeToken(parser, HASH_LPAREN, "Expected '(' after function name")) { return NULL; }
    
    node_t* params     = NULL;
    node_t* last_param = NULL;
    
    while (!MatchToken(parser, HASH_RPAREN) && !MatchToken(parser, HASH_EOF))
    {
        if (CUR_TYPE != ARG_VAR)
        {
            PrintError(parser, CUR_TOKEN, "Expected parameter name");
            free(params);
            return NULL;
        }
        
        node_t* param = ParseVar(parser);
        if (IS_BAD_PTR(param)) { free(params); return NULL; }
        
        if (IS_BAD_PTR(params))
        {
            params     = param;
            last_param = param;
        }
        else
        {
            last_param->right = param;
            last_param        = param;
        }
        
        if (!MatchToken(parser, HASH_COMMA)) { break; }
    }
    
    if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')' after parameters")) { free(params); return NULL; }
    
    if (!ConsumeToken(parser, HASH_LBRACE, "Expected '{' before function body")) { free(params); return NULL; }
    
    node_t* body = ParseBlock(parser);
    if (IS_BAD_PTR(body)) { free(params); return NULL; }
    
    if (!ConsumeToken(parser, HASH_RBRACE, "Expected '}' after function body")) { free(params); free(body); return NULL; }
    
    node_t* func_node = FUNC_(name_token->start);
    if (IS_BAD_PTR(func_node)) { free(params); free(body); return NULL; }
    
    func_node->left  = params;
    func_node->right = body;
    
    if (CUR_NAME_TABLE) { htInsert(CUR_NAME_TABLE, name_token->start); }
    
    return func_node;
}


node_t* ParseStatement(parser_t *parser)
{
    if (CUR_TYPE == ARG_OP)
    {
        switch (CUR_HASH)
        {
            case HASH_IF:
                return ParseIf(parser);
            case HASH_WHILE:
                return ParseWhile(parser);
            case HASH_RETURN:
                return ParseReturn(parser);        
            default:
                break;
        }
    }
    
    node_t* expr = ParseExpression(parser);
    ConsumeToken(parser, HASH_SEMICOLON, "Expected ';' after statement");
    return expr;
}


node_t* ParseExpression(parser_t* parser)
{
    node_t* node = ParseTerm(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(parser, HASH_ADD) || MatchToken(parser, HASH_SUB))
    {
        hash_t op_hash = PREV_HASH;
        
        node_t* right_node = ParseTerm(parser);
        if (IS_BAD_PTR(right_node)) { free(node); return NULL; }
        
        node_t* op_node = OP_(op_hash);
        if (IS_BAD_PTR(op_node)) { free(node); free(right_node); return NULL; }
        
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
        hash_t op_hash = parser->lexer->tokens->data[parser->lexer->cur_token - 1]->hash;
        
        node_t* right_node = ParseFactor(parser);
        if (IS_BAD_PTR(right_node)) { free(node); return NULL; }
        
        node_t* op_node = OP_(op_hash);
        if (IS_BAD_PTR(op_node)) { free(node); free(right_node); return NULL; }
        
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
        if (IS_BAD_PTR(right_node)) { free(node); return NULL; }
        
        node_t* op_node = OP_(HASH_POW);
        if (IS_BAD_PTR(op_node)) { free(node); free(right_node); return NULL; }
        
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
        case ARG_FUNC:
            return ParseFuncCall(parser);
        case ARG_OP:
        default:
            break;
    }

    if (MatchToken(parser, HASH_LPAREN))
    {
        node_t* expr = ParseExpression(parser);
        if (IS_BAD_PTR(expr)) return NULL;
        
        if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')' after expression")) { free(expr); return NULL; }
        
        return expr;
    }
    
    PrintError(parser, CUR_TOKEN, "Expected number, variable, function, or '('");
    return NULL;
}


node_t* ParseIf(parser_t* parser)
{
    MatchToken(parser, HASH_IF);
    
    if (!ConsumeToken(parser, HASH_LPAREN, "Expected '(' after 'if'")) { return NULL; }
    
    node_t* condition = ParseExpression(parser);
    if (IS_BAD_PTR(condition)) return NULL;
    
    if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')' after condition")) { free(condition); return NULL; }
    
    if (!ConsumeToken(parser, HASH_LBRACE, "Expected '{' before if body")) { free(condition); return NULL; }
    
    node_t* if_body = ParseBlock(parser);
    if (IS_BAD_PTR(if_body)) { free(condition); return NULL; }
    
    if (!ConsumeToken(parser, HASH_RBRACE, "Expected '}' after if body")) { free(condition); free(if_body); return NULL; }
    
    node_t* else_body = NULL;
    if (MatchToken(parser, HASH_ELSE))
    {
        if (!ConsumeToken(parser, HASH_LBRACE, "Expected '{' before else body")) { free(condition); free(if_body); return NULL; }
        
        else_body = ParseBlock(parser);
        if (IS_BAD_PTR(else_body)) { free(condition); free(if_body); return NULL; }
        
        if (!ConsumeToken(parser, HASH_RBRACE, "Expected '}' after else body")) { free(condition); free(if_body); free(else_body); return NULL; }
    }
    
    node_t* if_node = OP_(HASH_IF);
    if (IS_BAD_PTR(if_node)) { free(condition); free(if_body); free(else_body); return NULL; }
    
    if_node->left = condition;
    
    node_t* branches = OP_(HASH_BLOCK);
    if (IS_BAD_PTR(branches)) { free(if_node); free(condition); free(if_body); free(else_body); return NULL; }
    
    branches->left  = if_body;
    branches->right = else_body;
    
    if_node->right = branches;
    
    return if_node;
}


node_t* ParseWhile(parser_t* parser)
{
    MatchToken(parser, HASH_WHILE);
    
    if (!ConsumeToken(parser, HASH_LPAREN, "Expected '(' after 'while'")) { return NULL; }
    
    node_t* condition = ParseExpression(parser);
    if (IS_BAD_PTR(condition)) return NULL;
    
    if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')' after condition")) { free(condition); return NULL; }
    
    if (!ConsumeToken(parser, HASH_LBRACE, "Expected '{' before while body")) { free(condition); return NULL; }
    
    node_t* body = ParseBlock(parser);
    if (IS_BAD_PTR(body)) { free(condition); return NULL; }
    
    if (!ConsumeToken(parser, HASH_RBRACE, "Expected '}' after while body")) { free(condition); free(body); return NULL; }
    
    node_t* while_node = OP_(HASH_WHILE);
    if (IS_BAD_PTR(while_node)) { free(condition); free(body); return NULL; }
    
    while_node->left  = condition;
    while_node->right = body;
    
    return while_node;
}


node_t* ParseVarDecl(parser_t* parser)
{
    if (!MatchToken(parser, HASH_INIT))
    {
        PrintError(parser, CUR_TOKEN, "Expected 'init' keyword for variable declaration");
        return NULL;
    }
    
    if (CUR_TYPE != ARG_VAR)
    {
        PrintError(parser, CUR_TOKEN, "Expected variable name after 'init'");
        return NULL;
    }
    
    token_t* var_token = CUR_TOKEN;
    MatchToken(parser, var_token->hash);
    
    if (MatchToken(parser, HASH_SEMICOLON))
    {
        node_t* decl_node = OP_(HASH_INIT);
        if (IS_BAD_PTR(decl_node)) return NULL;
        
        node_t* var_node = VAR_(CUR_START);
        if (IS_BAD_PTR(var_node)) { free(decl_node); return NULL; }
        
        decl_node->left = var_node;
        
        if (CUR_NAME_TABLE) { htInsert(CUR_NAME_TABLE, var_token->start); }
        
        return decl_node;
    }
    
    if (!ConsumeToken(parser, HASH_EQ, "Expected '=' or ';' after variable name")) { return NULL; }
    
    node_t* init_value = ParseExpression(parser);
    if (IS_BAD_PTR(init_value)) return NULL;
    
    if (!ConsumeToken(parser, HASH_SEMICOLON, "Expected ';' after initialization")) { free(init_value); return NULL; }
    
    node_t* decl_node = OP_(HASH_INIT);
    if (IS_BAD_PTR(decl_node)) { free(init_value); return NULL; }
    
    node_t* var_node = VAR_(CUR_START);
    if (IS_BAD_PTR(var_node)) { free(decl_node); free(init_value); return NULL; }
    
    decl_node->left  = var_node;
    decl_node->right = init_value;
    
    if (CUR_NAME_TABLE) { htInsert(CUR_NAME_TABLE, var_token->start); }
    
    return decl_node;
}


node_t* ParseAssignment(parser_t* parser)
{
    if (CUR_TYPE != ARG_VAR)
    {
        PrintError(parser, CUR_TOKEN, "Expected variable name for assignment");
        return NULL;
    }
    
    MatchToken(parser, CUR_HASH);
    
    if (!ConsumeToken(parser, HASH_EQ, "Expected '=' after variable name")) { return NULL; }
    
    node_t* value = ParseExpression(parser);
    if (IS_BAD_PTR(value)) return NULL;
    
    ConsumeToken(parser, HASH_SEMICOLON, "Expected ';' after assignment");
    
    node_t* assign_node = OP_(HASH_EQ);
    if (IS_BAD_PTR(assign_node)) { free(value); return NULL; }
    
    node_t* var_node = VAR_(CUR_START);
    if (IS_BAD_PTR(var_node)) { free(assign_node); free(value); return NULL; }
    
    assign_node->left  = var_node;
    assign_node->right = value;
    
    return assign_node;
}


node_t* ParseReturn(parser_t* parser)
{
    MatchToken(parser, HASH_RETURN);
    
    node_t* value = ParseExpression(parser);
    if (IS_BAD_PTR(value)) return NULL;
    
    ConsumeToken(parser, HASH_SEMICOLON, "Expected ';' after return");
    
    node_t* return_node = OP_(HASH_RETURN);
    if (IS_BAD_PTR(return_node)) { free(value); return NULL; }
    
    return_node->left = value;
    
    return return_node;
}


node_t* ParseFuncCall(parser_t* parser)
{
    if (CUR_TYPE != ARG_FUNC && CUR_TYPE != ARG_VAR)
    {
        PrintError(parser, CUR_TOKEN, "Expected function name");
        return NULL;
    }
    
    token_t* func_token = CUR_TOKEN;
    MatchToken(parser, func_token->hash);
    
    if (!ConsumeToken(parser, HASH_LPAREN, "Expected '(' after function name")){ return NULL; }
    
    node_t* args = NULL;
    node_t* last_arg = NULL;
    
    if (!MatchToken(parser, HASH_RPAREN))
    {
        do {
            node_t* arg = ParseExpression(parser);
            if (IS_BAD_PTR(arg)) { free(args); return NULL; }
            
            if (IS_BAD_PTR(args))
            {
                args     = arg;
                last_arg = arg;
            }
            else
            {
                last_arg->right = arg;
                last_arg        = arg;
            }
        } while (MatchToken(parser, HASH_COMMA));
        
        if (!ConsumeToken(parser, HASH_RPAREN, "Expected ')' after arguments")) { free(args); return NULL; }
    }
    
    node_t* call_node = FUNC_(func_token->start);
    if (IS_BAD_PTR(call_node)) { free(args); return NULL; }
    
    call_node->left = args;
    
    return call_node;
}


node_t* ParseBlock(parser_t* parser)
{    
    node_t* block = OP_(HASH_BLOCK);
    if (IS_BAD_PTR(block)) return NULL;
    
    node_t* last_stmt = NULL;
    
    while (!MatchToken(parser, HASH_RBRACE) && !MatchToken(parser, HASH_EOF))
    {
        node_t* stmt = ParseStatement(parser);
        if (IS_BAD_PTR(stmt)) { free(block); return NULL; }
        
        if (IS_BAD_PTR(block->left))
        {
            block->left = stmt;
            last_stmt   = stmt;
        }
        else
        {
            last_stmt->right = stmt;
            last_stmt        = stmt;
        }
    }
    
    return block;
}


node_t* ParseVar(parser_t* parser)
{
    if (CUR_TYPE != ARG_VAR)
    {
        PrintError(parser, CUR_TOKEN, "Expected variable");
        return NULL;
    }
    
    MatchToken(parser, CUR_HASH);
        
    return NewNode(ARG_VAR, CUR_HASH, NULL, NULL);
}


node_t* ParseNum(parser_t* parser)
{
    if (CUR_TYPE != ARG_NUM)
    {
        PrintError(parser, CUR_TOKEN, "Expected number");
        return NULL;
    }
    
    token_t* num_token = CUR_TOKEN;
    MatchToken(parser, num_token->hash);
    
    double value = 0.0;
    int num_digits = 0;
    sscanf(num_token->start, "%lg%n", &value, &num_digits);
    
    node_t* num_node = NUM_(value);
    return num_node;
}