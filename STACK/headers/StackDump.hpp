#include "stack.hpp"

#ifndef STACK_DUMP_HPP
#define STACK_DUMP_HPP


template <typename stackElem_T>
StackErr_t StackDump(stk_t<stackElem_T> *stk, const char *file, const char * func, int line)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )

    #define PRINT_ERRCODE_(code) do { if (ERR_CHECK(code) == STK_ERROR) LOG(ERROR, #code); } while(0)

    PRINT_ERRCODE_(STK_WRONG_CANARY);
    PRINT_ERRCODE_(STK_WRONG_HASH);
    PRINT_ERRCODE_(STK_BAD_DATA_PTR);
    PRINT_ERRCODE_(STK_WRONG_SIZE);
    PRINT_ERRCODE_(STK_WRONG_CAPACITY);
    PRINT_ERRCODE_(STK_WRONG_REALLOC);
    PRINT_ERRCODE_(STK_WRONG_POISON_VAL);
    PRINT_ERRCODE_(STK_DATA_OVERFLOW);
    PRINT_ERRCODE_(STK_ACCESS_EMPTY_DATA);

    #undef PRINT_ERRCODE_

    if (ERR_CHECK(STK_BAD_DATA_PTR) == STK_NO_ERRORS && stk->capacity > 2 && stk->size > 2)
    {
        fprintf(LogFile, "---------------------------------------------------------------------------------------------------------------------------\n");

        char str_value[32] = {0};

        fprintf(LogFile, "DUMP stack \"%s\" [%p] from %s() at %s:%d:\n\n", stk->id.name, stk, func, file, line);

        fprintf(LogFile, "STK_POISON = %d\n\n", STK_POISON);

        fprintf(LogFile, "STK_CANARY_1 = %zu\n", (size_t)STK_CANARY_1);
        fprintf(LogFile, "STK_CANARY_2 = %zu\n", (size_t)STK_CANARY_2);  
 
        fprintf(LogFile, "%s {\n", stk->id.name);

        snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(stk->canary_1), stk->canary_1);
        fprintf(LogFile, "\tcanary_1 = %s; (STK_CANARY_1)\n", str_value);

        fprintf(LogFile, "\tdata = {\n");

        for (size_t i = 0; i < (size_t)stk->capacity; ++i)
        {
            snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(stk->data[i]), stk->data[i]);
            fprintf(LogFile, "\t\t[%zu] = %s, %s\n", i, str_value, (i < (size_t)stk->size) ? "": "(STK_POISON)");
        }
        fprintf(LogFile, "\t\t};\n");

        fprintf(LogFile, "\tsize = %zd;\n", stk->size);
        fprintf(LogFile, "\tcapacity = %zd;\n", stk->capacity);
        fprintf(LogFile, "\thash = %zx;\n", stk->hash);

        snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(stk->canary_2), stk->canary_2);
        fprintf(LogFile, "\tcanary_2 = %s; (STK_CANARY_2)\n", str_value);

        fprintf(LogFile, "}\n");

        fprintf(LogFile, "---------------------------------------------------------------------------------------------------------------------------\n");
    }
        
    return STK_SUCCESS;
}

#endif