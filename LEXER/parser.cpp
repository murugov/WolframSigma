#include "parser.h"
#include "wolfram.h"
#include "OpInstrSet.cpp"


parser_t *parserCtor(lexer_t* lexer)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    parser_t* parser = (parser_t*)calloc(1, sizeof(parser_t));
    if (IS_BAD_PTR(parser)) return NULL;
    
    parser->lexer      = lexer;
    parser->cur_token  = NULL;
    parser->prev_token = NULL;

    AdvanceToken(parser);
    
    return parser;
}


void parserDtor(parser_t* parser)
{
    if (!IS_BAD_PTR(parser))
    {
        if (parser->cur_token)
            free(parser->cur_token);
        if (parser->prev_token)
            free(parser->prev_token);
        free(parser);
    }
}


void FreeLines(char **arr_ptr, int count_lines)
{
    ON_DEBUG( if (IS_BAD_PTR(arr_ptr)) return;)
    
    for (int i = 0; i < count_lines; ++i)
        free(arr_ptr[count_lines]);
}


void AdvanceToken(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return; )

    parser->prev_token = parser->cur_token;
    parser->cur_token = NextToken(parser->lexer);
}


int MatchToken(parser_t* parser, type_t type)
{
    if (CheckType(parser, type)) { AdvanceToken(parser); return 1; }
    return 0;
}


int CheckType(parser_t* parser, type_t type)
{
    if (parser->cur_token->type == TOKEN_EOF) return 0;
    return parser->cur_token->type == type;
}


token_t* ConsumeToken(parser_t* parser, type_t type, const char* error_msg)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    if (CheckType(parser, type))
    {
        token_t* token = parser->cur_token;
        AdvanceToken(parser);
        return token;
    }

    PrintError(parser, parser->cur_token, error_msg);
    return NULL;
}


node_t* ParseGeneral(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    node_t* node = ParseExpression(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    if (!MatchToken(parser, TOKEN_DOLLAR))
    {
        PrintError(parser, parser->cur_token, "Expected '$' at end of expression");
        FreeNodes(node);
        return NULL;
    }
    
    set_parents(node, NULL);
    return node;
}


node_t* ParseExpression(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    node_t* node = ParseTerm(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(parser, TOKEN_ADD) || MatchToken(parser, TOKEN_SUB))
    {
        type_t op_type = parser->prev_token->type;
        
        node_t* right = ParseTerm(parser);
        if (IS_BAD_PTR(right)) { FreeNodes(node); return NULL; }
        
        if (op_type == TOKEN_ADD)
            node = ADD_(node, right);
        else
            node = SUB_(node, right);
    }
    
    return node;
}


node_t* ParseTerm(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    node_t* node = ParseFactor(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(parser, TOKEN_MUL) || MatchToken(parser, TOKEN_DIV))
    {
        type_t op_type = parser->prev_token->type;
        
        node_t* right = ParseFactor(parser);
        if (IS_BAD_PTR(right)) { FreeNodes(node); return NULL; }
        
        if (op_type == TOKEN_MUL)
            node = MUL_(node, right);
        else
            node = DIV_(node, right);
    }
    
    return node;
}


node_t* ParseFactor(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    node_t* node = ParsePrimary(parser);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(parser, TOKEN_POW))
    {
        node_t* right = ParsePrimary(parser);
        if (IS_BAD_PTR(right)) { FreeNodes(node); return NULL; }
        
        node = POW_(node, right);
    }
    
    return node;
}


node_t* ParsePrimary(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )
    // printf("ParsePrimary begin: %d\n", parser->cur_token->type);

    if (MatchToken(parser, TOKEN_NUM))
        return ParseNumber(parser);
    
    if (MatchToken(parser, TOKEN_VAR))
        return ParseVariable(parser);
    
    if (CheckType(parser, TOKEN_FUNC))
        return ParseFunctionCall(parser);
    
    if (MatchToken(parser, TOKEN_LPAREN))
    {
        node_t* node_expression = ParseExpression(parser);
        if (IS_BAD_PTR(node_expression)) return NULL;
        
        if (!ConsumeToken(parser, TOKEN_RPAREN, "Expected ')' after expression"))
        {
            FreeNodes(node_expression);
            return NULL;
        }
        
        return node_expression;
    }
    
    PrintError(parser, parser->cur_token, "Expected expression");
    return NULL;
}

node_t* ParseFunctionCall(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    token_t* func_token = ConsumeToken(parser, TOKEN_FUNC, "Expected function name");
    if (IS_BAD_PTR(func_token)) return NULL;
    
    char func_name[256];
    strncpy(func_name, func_token->start, (size_t)func_token->length);
    func_name[func_token->length] = '\0';
    
    hash_t hash_item = HashStr(func_name);
    size_t index = 0;
    
    if (HashSearch(hash_item, &index) != TREE_SUCCESS)
    {
        PrintError(parser, func_token, "Unknown function");
        return NULL;
    }
    
    if (!ConsumeToken(parser, TOKEN_LPAREN, "Expected '(' after function name")) return NULL;
    
    node_t* arg1 = ParseExpression(parser);
    if (IS_BAD_PTR(arg1)) return NULL;
    
    if (op_instr_set[index].num_args == 1)
    {
        if (!ConsumeToken(parser, TOKEN_RPAREN, "Expected ')' after argument"))
        {
            FreeNodes(arg1);
            return NULL;
        }
        
        node_t* func_node = VAR_(func_name);
        if (IS_BAD_PTR(func_node)) { FreeNodes(arg1); return NULL; }
        
        func_node->type = ARG_OP;
        func_node->right = arg1;
        return func_node;
        
    }
    else if (op_instr_set[index].num_args == 2)
    {
        if (!ConsumeToken(parser, TOKEN_COMMA, "Expected ',' for second argument"))
        {
            FreeNodes(arg1);
            return NULL;
        }
        
        node_t* arg2 = ParseExpression(parser);
        if (IS_BAD_PTR(arg2)) { FreeNodes(arg1); return NULL; }
        
        if (!ConsumeToken(parser, TOKEN_RPAREN, "Expected ')' after second argument"))
        {
            FreeNodes(arg1);
            FreeNodes(arg2);
            return NULL;
        }
        
        node_t* func_node = VAR_(func_name);
        if (IS_BAD_PTR(func_node)) { FreeNodes(arg1); FreeNodes(arg2); return NULL; }
        
        func_node->type = ARG_OP;
        func_node->left = arg1;
        func_node->right = arg2;
        return func_node;
        
    }

    PrintError(parser, func_token, "Function has invalid number of arguments");
    FreeNodes(arg1);
    return NULL;
}


node_t* ParseVariable(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    token_t* var_token = parser->prev_token;
    
    char var_name[256];                                                     // DEFINE
    strncpy(var_name, var_token->start, (size_t)var_token->length);
    var_name[var_token->length] = '\0';
    
    node_t* var_node = VAR_(var_name);
    IS_USED_VARS(var_name, var_node);
    
    if (IS_BAD_PTR(var_node))
    {
        PrintError(parser, var_token, "Failed to create variable node");
        return NULL;
    }
    
    return var_node;
}


node_t* ParseNumber(parser_t* parser)
{
    ON_DEBUG( if (IS_BAD_PTR(parser)) return NULL; )

    token_t* num_token = parser->prev_token;
    
    node_t *num_node = NUM_(0);
    if (IS_BAD_PTR(num_node)) return NULL;
    
    int num_digits = 0;
    if (sscanf(num_token->start, "%lg%n", &(num_node->item.num), &num_digits) != 1) { free(num_node); return NULL; }
    
    return num_node;
}


void PrintError(parser_t* parser, token_t* token, const char* message)
{
    ON_DEBUG( if (IS_BAD_PTR(parser) || IS_BAD_PTR(token) || IS_BAD_PTR(message)) return; )
    if (parser->prev_token && parser->prev_token->type == TOKEN_EOF)
        printf(ANSI_COLOR_RED "Error at end: %s\n" ANSI_COLOR_RESET, message);
    else if (token)
        printf(ANSI_COLOR_RED "Error at %d:%d: %s\n" ANSI_COLOR_RESET, token->line, token->col, message);
    else
        printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, message);
}