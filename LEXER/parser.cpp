#include "parser.hpp"
#include "wolfram.hpp"
#include "OpInstrSet.cpp"


// void AdvanceToken(lexer_t* lexer)
// {
//     ON_DEBUG( if (IS_BAD_PTR(lexer)) return; )

//     StackPush(lexer->tokens, NextToken(lexer));
//     lexer->cur_token++;                                         // maybe does't need
// }


int MatchToken(lexer_t* lexer, type_t type)
{
    // if (CheckType(lexer, type)) { AdvanceToken(lexer); return 1; }
    return 0;
}


int CheckType(lexer_t* lexer, type_t type)
{
    type_t token_type = (lexer->tokens->data)[lexer->tokens->size]->type;

    if (token_type == TOKEN_EOF) return 0;
    return token_type == type;
}


token_t* ConsumeToken(lexer_t* lexer, type_t type, const char* error_msg)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    if (CheckType(lexer, type))
    {
        token_t* token = lexer->tokens->data[lexer->cur_token];
        // AdvanceToken(lexer);
        return token;
    }

    // PrintError(parser, parser->cur_token, error_msg);
    return NULL;
}


node_t* ParseGeneral(lexer_t* lexer)                        // rename ParseAst
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    node_t* node = ParseExpression(lexer);
    if (IS_BAD_PTR(node)) return NULL;
    
    if (!MatchToken(lexer, TOKEN_DOLLAR))
    {
        // PrintError(parser, parser->cur_token, "Expected '$' at end of expression");
        FreeNodes(node);
        return NULL;
    }
    
    set_parents(node, NULL);
    return node;
}


node_t* ParseExpression(lexer_t* lexer)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    node_t* node = ParseTerm(lexer);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(lexer, TOKEN_ADD) || MatchToken(lexer, TOKEN_SUB))
    {
        type_t op_type = (lexer->tokens->data)[lexer->tokens->size - 1]->type;  // -1?
        
        node_t* right = ParseTerm(lexer);
        if (IS_BAD_PTR(right)) { FreeNodes(node); return NULL; }
        
        if (op_type == TOKEN_ADD)
            node = ADD_(node, right);
        else
            node = SUB_(node, right);
    }
    
    return node;
}


node_t* ParseTerm(lexer_t* lexer)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    node_t* node = ParseFactor(lexer);
    if (IS_BAD_PTR(node)) return NULL;
    
    while (MatchToken(lexer, TOKEN_MUL) || MatchToken(lexer, TOKEN_DIV))
    {
        type_t op_type = (lexer->tokens->data)[lexer->tokens->size - 1]->type;
        
        node_t* right = ParseFactor(lexer);
        if (IS_BAD_PTR(right)) { FreeNodes(node); return NULL; }
        
        if (op_type == TOKEN_MUL)
            node = MUL_(node, right);
        else
            node = DIV_(node, right);
    }
    
    return node;
}


node_t* ParseFactor(lexer_t* parser)
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


node_t* ParsePrimary(lexer_t* parser)
{    
    if (MatchToken(parser, TOKEN_NUM))
        return ParseNumber(parser);
    
    if (MatchToken(parser, TOKEN_VAR))
        return ParseVariable(parser);
    
    if (CheckType(parser, TOKEN_FUNC))
        return ParseFunc(parser);
    
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
    
    // PrintError(parser, parser->cur_token, "Expected expression");
    return NULL;
}

node_t* ParseFunc(lexer_t* lexer)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    token_t* func_token = ConsumeToken(lexer, TOKEN_FUNC, "Expected function name");
    if (IS_BAD_PTR(func_token)) return NULL;
    
    char func_name[256];
    strncpy(func_name, func_token->start, (size_t)func_token->length);
    func_name[func_token->length] = '\0';
    
    hash_t hash_item = HashStr(func_name);
    size_t index = 0;
    
    if (HashSearch(hash_item, &index) != TREE_SUCCESS)
    {
        PrintError(lexer, func_token, "Unknown function");
        return NULL;
    }
    
    if (!ConsumeToken(lexer, TOKEN_LPAREN, "Expected '(' after function name")) return NULL;
    
    node_t* arg1 = ParseExpression(lexer);
    if (IS_BAD_PTR(arg1)) return NULL;
    
    if (op_instr_set[index].num_args == 1)
    {
        if (!ConsumeToken(lexer, TOKEN_RPAREN, "Expected ')' after argument"))
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
        if (!ConsumeToken(lexer, TOKEN_COMMA, "Expected ',' for second argument"))
        {
            FreeNodes(arg1);
            return NULL;
        }
        
        node_t* arg2 = ParseExpression(lexer);
        if (IS_BAD_PTR(arg2)) { FreeNodes(arg1); return NULL; }
        
        if (!ConsumeToken(lexer, TOKEN_RPAREN, "Expected ')' after second argument"))
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

    PrintError(lexer, func_token, "Function has invalid number of arguments");
    FreeNodes(arg1);
    return NULL;
}


node_t* ParseVariable(lexer_t* lexer)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    token_t* var_token = (lexer->tokens->data)[lexer->tokens->size - 1];
    
    char var_name[256];                                                     // DEFINE
    strncpy(var_name, var_token->start, (size_t)var_token->length);
    var_name[var_token->length] = '\0';
    
    node_t* var_node = VAR_(var_name);
    IS_USED_VARS(var_name, var_node);
    
    if (IS_BAD_PTR(var_node))
    {
        PrintError(lexer, var_token, "Failed to create variable node");
        return NULL;
    }
    
    return var_node;
}


node_t* ParseNumber(lexer_t* lexer)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return NULL; )

    token_t* num_token = (lexer->tokens->data)[lexer->tokens->size - 1];
    
    node_t *num_node = NUM_(0);
    if (IS_BAD_PTR(num_node)) return NULL;
    
    int num_digits = 0;
    if (sscanf(num_token->start, "%lg%n", &(num_node->item.num), &num_digits) != 1) { free(num_node); return NULL; }
    
    return num_node;
}


void PrintError(lexer_t* lexer, token_t* token, const char* message)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer) || IS_BAD_PTR(token) || IS_BAD_PTR(message)) return; )
    if (lexer->tokens->data[lexer->cur_token - 1] && (lexer->tokens->data[lexer->cur_token - 1])->type== TOKEN_EOF)
        printf(ANSI_COLOR_RED "Error at end: %s\n" ANSI_COLOR_RESET, message);
    else if (token)
        printf(ANSI_COLOR_RED "Error at %d:%d: %s\n" ANSI_COLOR_RESET, token->line, token->col, message);
    else
        printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, message);
}