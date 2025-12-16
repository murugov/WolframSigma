#include "stack.hpp"

#ifndef STACK_DUMP_HPP
#define STACK_DUMP_HPP


template <typename stackElem_T>
StackErr_t StackDump(stk_t<stackElem_T>* ON_DEBUG(stk), const char* ON_DEBUG(file), const char* ON_DEBUG(func), int ON_DEBUG(line))
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )

    #define PRINT_ERRCODE_(code) do { if (ERR_CHECK(code) == STK_ERROR) LOG(ERROR, #code); } while(0)

    ON_DEBUG(

        PRINT_ERRCODE_(STK_WRONG_CANARY);
        PRINT_ERRCODE_(STK_WRONG_HASH);
        PRINT_ERRCODE_(STK_BAD_DATA_PTR);
        PRINT_ERRCODE_(STK_WRONG_SIZE);
        PRINT_ERRCODE_(STK_WRONG_CAPACITY);
        PRINT_ERRCODE_(STK_WRONG_REALLOC);
        PRINT_ERRCODE_(STK_WRONG_POISON_VAL);
        PRINT_ERRCODE_(STK_DATA_OVERFLOW);
        PRINT_ERRCODE_(STK_ACCESS_EMPTY_DATA);
        
        if (ERR_CHECK(STK_BAD_DATA_PTR) == STK_NO_ERRORS && stk->capacity > 2 && stk->size > 2)
        {
            fprintf(LogFile, "---------------------------------------------------------------------------------------------------------------------------\n");
    
            char str_value[32] = {0};
    
            fprintf(LogFile, "DUMP stack \"%s\" [%p] from %s() at %s:%d:\n\n", stk->id.name, stk, func, file, line);
    
            fprintf(LogFile, "STK_POISON = %d\n\n", STK_POISON);
    
            fprintf(LogFile, "STK_CANARY_1 = %zu\n", (size_t)STK_CANARY_1);
            fprintf(LogFile, "STK_CANARY_2 = %zu\n", (size_t)STK_CANARY_2);  
            fprintf(LogFile, "STK_CANARY_3 = %zu\n", (size_t)STK_CANARY_3);
            fprintf(LogFile, "STK_CANARY_4 = %zu\n\n", (size_t)STK_CANARY_4);
    
            fprintf(LogFile, "%s {\n", stk->id.name);
    
            snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(stk->canary_1), stk->canary_1);
            fprintf(LogFile, "\tcanary_1 = %s; (STK_CANARY_1)\n", str_value);
    
            fprintf(LogFile, "\tdata = {\n");
    
            if (sizeof(stk_canary_t) == sizeof(stackElem_T))
            {
                snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(stk->data[0]), stk->data[0]);
                fprintf(LogFile, "\t\t[0] = %s, (STK_CANARY_3)\n", str_value);
            }
            else
            {
                stk_canary_t start_canary = 0;
                memcpy((void*)(&start_canary), (const void*)(&stk->data[0]), sizeof(stk_canary_t));
                snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(start_canary), start_canary);
                fprintf(LogFile, "\t\t[0-%zu] = %s, (STK_CANARY_3)\n", sizeof(stk_canary_t) / sizeof(stackElem_T) - 1, str_value);
            }
    
            for (size_t i = sizeof(stk_canary_t) / sizeof(stackElem_T); i < (size_t)stk->capacity - sizeof(stk_canary_t) / sizeof(stackElem_T); ++i)
            {
                snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(stk->data[i]), stk->data[i]);
                fprintf(LogFile, "\t\t[%zu] = %s, %s\n", i, str_value, (i < (size_t)stk->size - sizeof(stk_canary_t) / sizeof(stackElem_T)) ? "": "(STK_POISON)");
            }
    
            if (sizeof(stk_canary_t) == sizeof(stackElem_T))
            {
                snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(stk->data[(size_t)stk->capacity - sizeof(stk_canary_t) / sizeof(stackElem_T)]), stk->data[(size_t)stk->capacity - sizeof(stk_canary_t) / sizeof(stackElem_T)]);
                fprintf(LogFile, "\t\t[%zu] = %s, (STK_CANARY_4)\n", (size_t)stk->capacity - sizeof(stk_canary_t) / sizeof(stackElem_T), str_value);
            }
            else
            {
                stk_canary_t end_canary = 0;
                memcpy((void*)(&end_canary), (const void*)(&stk->data[(size_t)stk->capacity - sizeof(stk_canary_t) / sizeof(stackElem_T)]), sizeof(stk_canary_t));
                snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(end_canary), end_canary);
                fprintf(LogFile, "\t\t[%zu-%zu] = %s, (STK_CANARY_4)\n", (size_t)stk->capacity - sizeof(stk_canary_t) / sizeof(stackElem_T), (size_t)stk->capacity - 1, str_value);
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
    )

    #undef PRINT_ERRCODE_
        
    return STK_SUCCESS;
}

#endif