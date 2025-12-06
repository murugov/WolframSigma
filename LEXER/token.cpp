#include "token.h"


char **DataReader(FILE *SourceFile, char *buffer, int *count_line)
{
    ON_DEBUG(if (IS_BAD_PTR(SourceFile)) { LOG(ERROR, "TOKEN_ERROR"); return NULL;})

    size_t len_buffer = 0;
    char **arr_ptr = TXTreader(SourceFile, buffer, &len_buffer, count_line, NULL);
    RemoveComments(arr_ptr, count_line);

    for (int i = 0; i < *count_line; ++i)
        printf("%s", arr_ptr[i]);
    printf("\n");    

    return arr_ptr;
}


void RemoveComments(char **arr_ptr, int *count_line)
{
    for (int i = 0; i < *count_line; ++i)
    {
        char* sharp = strchr(arr_ptr[i], '#');
        if (sharp != NULL) *sharp = '\0';
    }
}