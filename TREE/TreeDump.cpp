#include "list.h"
#include "colors.h"

#define LOG_FILE "LIST/ReportFiles/LogFile.log"


ListErr_t ListDump(list_t *list, const char* FuncName, const char *file, const char *func, size_t line)
{
    ON_DEBUG(
        if (IS_BAD_PTR(list)) return LST_ERROR;
        if (IS_BAD_PTR(file)) return LST_ERROR;
        if (IS_BAD_PTR(func)) return LST_ERROR;
    )

    static bool first_call = true;
    const char *mode = first_call ? "w" : "a";
    first_call = false;

    FILE *LogFile = fopen(LOG_FILE, mode);

    if (IS_BAD_PTR(LogFile))
    {
        printf(ANSI_COLOR_RED "Bad pointer to %s\n" ANSI_COLOR_RESET, LOG_FILE);
        return LST_ERROR;
    }

    if (IS_BAD_PTR(list))
    {
        fprintf(LogFile, "---------------------------------------------------------------------------------------------------------------------------\n\n");
        
        fprintf(LogFile, "ListDump() from %s at %s:%zu: Bad list at function\n", func, file, line);
        fprintf(LogFile, "List <int> [NULL] ERROR! BAD_LIST_PTR\n");

        fprintf(LogFile, "\n---------------------------------------------------------------------------------------------------------------------------\n");
    }

    else
    {
        fprintf(LogFile, "---------------------------------------------------------------------------------------------------------------------------\n\n");

        if (list->error == 0)
        {
            fprintf(LogFile, "ListDump() from %s at %s:%zu:\n", func, file, line);

            if (!IS_BAD_PTR(list->id.name) && !IS_BAD_PTR(list->id.file) && !IS_BAD_PTR(list->id.func))
                fprintf(LogFile, "List <int> \"%s\" [%p] from %s:%zu:", list->id.name + 1, list, list->id.file, list->id.line);
            
            else
                fprintf(LogFile, "List <int> [%p]", list);
        }

        else
        {
            fprintf(LogFile, "ListDump() from %s at %s:%zu: Bad list at function\n", func, file, line);
    
            if (!IS_BAD_PTR(list->id.name) && !IS_BAD_PTR(list->id.file) && !IS_BAD_PTR(list->id.func))
                fprintf(LogFile, "List <int> \"%s\" [%p] from %s:%zu: ERROR! ", list->id.name + 1, list, list->id.file, list->id.line);
            
            else
                fprintf(LogFile, "List <int> [%p] ERROR! ", list);
                    
            #define PRINT_ERRCODE_(code)  fprintf(LogFile, "%s", (ERR_CHECK(list, code)) ? #code " " : "")
        
            PRINT_ERRCODE_(WRONG_CANARY);
            PRINT_ERRCODE_(WRONG_HASH);
            PRINT_ERRCODE_(WRONG_CAPACITY);
            PRINT_ERRCODE_(WRONG_FIRST_FREE);
            PRINT_ERRCODE_(ACCESS_EMPTY_DATA);
            PRINT_ERRCODE_(WRONG_FREE_ITEM);
            PRINT_ERRCODE_(WRONG_LAST_FREE);
            PRINT_ERRCODE_(WRONG_NEXT_SUB);
            PRINT_ERRCODE_(WRONG_PREV_SUB);
            PRINT_ERRCODE_(INVALID_INDEX);
            PRINT_ERRCODE_(REALLOC_FAIL);
            PRINT_ERRCODE_(REV_REALLOC_FAIL);
            PRINT_ERRCODE_(DATA_OVERFLOW);
            PRINT_ERRCODE_(ACCESS_EMPTY_DATA);
            PRINT_ERRCODE_(GEN_GRAPH_FAIL);
            PRINT_ERRCODE_(GEN_DOT_FAIL);
            PRINT_ERRCODE_(GEN_HTML_FAIL);
        
            #undef PRINT_ERRCODE_
        }
    
        fprintf(LogFile, "\n\n");
    
        if (!IS_BAD_PTR(list->data) && !IS_BAD_PTR(list->next) && !IS_BAD_PTR(list->prev)
            && ERR_CHECK(list, WRONG_SIZE) == LST_SUCCESS && ERR_CHECK(list, WRONG_CAPACITY) == LST_SUCCESS
            && ERR_CHECK(list, DATA_OVERFLOW) == LST_SUCCESS && ERR_CHECK(list, ACCESS_EMPTY_DATA) == LST_SUCCESS)
        {
            fprintf(LogFile, "POISON = %d\n", POISON);
            fprintf(LogFile, "first_free = %zd\n\n", list->first_free);

            fprintf(LogFile, "data[%p]:\n", list->data);
            fprintf(LogFile, "\t[");
            fprintf(LogFile, "%d", list->data[0]);
            for (size_t i = 1; i < (size_t)list->capacity; ++i)
            {
                fprintf(LogFile, ", %d", list->data[i]);
            }
            fprintf(LogFile, "]\n\n");

            fprintf(LogFile, "next[%p]:\n", list->next);
            fprintf(LogFile, "\t[");
            fprintf(LogFile, "%zd", list->next[0]);
            for (size_t i = 1; i < (size_t)list->capacity; ++i)
            {
                fprintf(LogFile, ", %zd", list->next[i]);
            }
            fprintf(LogFile, "]\n\n");

            fprintf(LogFile, "prev[%p]:\n", list->prev);
            fprintf(LogFile, "\t[");
            fprintf(LogFile, "%zd", list->prev[0]);
            for (size_t i = 1; i < (size_t)list->capacity; ++i)
            {
                fprintf(LogFile, ", %zd", list->prev[i]);
            }
            fprintf(LogFile, "]\n");
        }
    
        fprintf(LogFile, "\n---------------------------------------------------------------------------------------------------------------------------\n\n");
    }

    fclose(LogFile);

    if (GenGraphs(list, FuncName)) 
    {
        perror("GenGraphs failed!!!");
        return LST_ERROR;
    }
    return LST_SUCCESS;
}

#undef LOG_FILE