#include "lexer.h"
#include "OpInstrSet.cpp"


node_t *GetGeneral(const char **lex_pos)
{
    node_t *node = GetExpress(lex_pos);
    if (node == NULL || **lex_pos != '$')
    {
        printf(ANSI_COLOR_RED "Syntax error!\n" ANSI_COLOR_RESET);
        printf("cur_pos: %s\n", *lex_pos);
        FreeNodes(node);
        return NULL;
    }
        
    set_parents(node, NULL);
    (*lex_pos)++;
    
    return node;
}


node_t *GetExpress(const char **lex_pos)
{
    node_t *node_1 = GetTerm(lex_pos);
    if (IS_BAD_PTR(node_1)) return NULL;

    node_t *node_res = node_1;
    while (**lex_pos == '+' || **lex_pos == '-')
    {
        int op = **lex_pos;
        (*lex_pos)++;
        node_t *node_2 = GetTerm(lex_pos);
        if (IS_BAD_PTR(node_2)) { FreeNodes(node_res); return NULL; }

        if (op == '+')
            node_res = ADD_(node_res, node_2);
        else
            node_res = SUB_(node_res, node_2);
    }

    return node_res;
}


node_t *GetTerm(const char **lex_pos)
{
    node_t *node_1 = GetPow(lex_pos);
    if (IS_BAD_PTR(node_1)) return NULL;

    node_t *node_res = node_1;
    while (**lex_pos == '*' || **lex_pos == '/')
    {
        int op = **lex_pos;
        (*lex_pos)++;
        node_t *node_2 = GetPow(lex_pos);
        if (IS_BAD_PTR(node_2)) { FreeNodes(node_res); return NULL; }

        if (op == '*')
            node_res = MUL_(node_res, node_2);
        else
            node_res = DIV_(node_res, node_2);
    }

    return node_res;
}


node_t *GetPow(const char **lex_pos)
{
    node_t *node_1 = GetFunc(lex_pos);
    if (IS_BAD_PTR(node_1)) return NULL;

    node_t *node_res = node_1;
    while (**lex_pos == '^')
    {
        (*lex_pos)++;
        node_t *node_2 = GetFunc(lex_pos);
        if (IS_BAD_PTR(node_2)) { FreeNodes(node_res); return NULL; }

        node_res = POW_(node_res, node_2);
    }

    return node_res;
}


node_t *GetFunc(const char **lex_pos)
{
    const char* saved_pos = *lex_pos;
    
    node_t *node_name = GetName(lex_pos);
    if (node_name == NULL) 
    {
        *lex_pos = saved_pos;
        return GetParen(lex_pos);
    }

    hash_t hash_item = HashStr(node_name->item.var);
    size_t index = 0;

    if (HashSearch(hash_item, &index) == TREE_SUCCESS)
    {
        
        if (**lex_pos != '(')
        {
            printf(ANSI_COLOR_RED "Expected '(' before function argument\n" ANSI_COLOR_RESET);
            return NULL;
        }
        (*lex_pos)++;
        
        node_name->type = ARG_OP;

        node_t *node_arg = GetExpress(lex_pos);
        if (IS_BAD_PTR(node_arg)) 
        { 
            FreeNodes(node_name); 
            return NULL; 
        }

        if (op_instr_set[index].num_args == 1)
        {
            node_name->right = node_arg;
            if (**lex_pos != ')')
            {
                printf(ANSI_COLOR_RED "Expected ')' after function argument\n" ANSI_COLOR_RESET);
                FreeNodes(node_name);
                FreeNodes(node_arg);
                return NULL;
            }
            (*lex_pos)++;

            return node_name;
        }
        else
        {
            if (**lex_pos != ',')
            {
                printf(ANSI_COLOR_RED "Expected ',' for binary function\n" ANSI_COLOR_RESET);
                FreeNodes(node_name);
                FreeNodes(node_arg);
                return NULL;
            }
            (*lex_pos)++;
            
            node_t *node_arg2 = GetExpress(lex_pos);
            if (IS_BAD_PTR(node_arg2)) 
            { 
                FreeNodes(node_name); 
                FreeNodes(node_arg);
                return NULL; 
            }
            
            if (**lex_pos != ')')
            {
                printf(ANSI_COLOR_RED "Expected ')' after second function argument\n" ANSI_COLOR_RESET);
                FreeNodes(node_name);
                FreeNodes(node_arg);
                FreeNodes(node_arg2);
                return NULL;
            }
            (*lex_pos)++;

            node_name->left  = node_arg;
            node_name->right = node_arg2;

            return node_name;
        }
    }

    return node_name;
}


bool isValidName(const char* name)
{
    if (IS_BAD_PTR(name) || name[0] == '\0' || !isalpha(name[0])) return false;
    
    if (!isalpha(name[0])) return false;
    
    for (int i = 1; name[i] != '\0'; ++i)
        if (!isalnum(name[i]) && name[i] != '_') return false;
    
    return true;
}


node_t *GetName(const char **lex_pos)
{
    int num_symbols = 0;
    char tmp[256] = {0};
    
    if (sscanf(*lex_pos, "%255[a-zA-Z0-9_]%n", tmp, &num_symbols) != 1) 
        return NULL;

    if (!isValidName(tmp)) return NULL;

    node_t *node_var = VAR_(tmp);
    if (IS_BAD_PTR(node_var)) return NULL;

    (*lex_pos) += num_symbols;
    return node_var;
}


node_t *GetParen(const char **lex_pos)
{
    if (**lex_pos == '(')
    {
        (*lex_pos)++;
        node_t *node = GetExpress(lex_pos);

        if (**lex_pos != ')')
        {
            printf(ANSI_COLOR_RED "Expected ')'\n" ANSI_COLOR_RESET);
            FreeNodes(node);
            return NULL;
        }
        (*lex_pos)++;
        return node;
    }

    const char* saved_pos = *lex_pos;
    node_t *node_name = GetName(lex_pos);
    if (node_name != NULL) return node_name;
    
    (*lex_pos) = saved_pos;
    return GetNum(lex_pos);
}


node_t *GetNum(const char **lex_pos)
{
    node_t *node_num = NUM_(0);
    if (IS_BAD_PTR(node_num)) return NULL;
    
    int num_digits = 0;
    if (sscanf(*lex_pos, "%lg%n", &(node_num->item.num), &num_digits) != 1)
    {
        free(node_num);
        return NULL;
    }
    
    (*lex_pos) += num_digits;
    return node_num;
}