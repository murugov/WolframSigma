#include "token.hpp"


char **DataReader(FILE *SourceFile, char *buffer, int *count_line)      // why is this here?
{
    ON_DEBUG(if (IS_BAD_PTR(SourceFile)) { LOG(ERROR, "TOKEN_ERROR"); return NULL;})

    size_t len_buffer = 0;
    char **arr_ptr = TXTreader(SourceFile, buffer, &len_buffer, count_line, NULL);
    RemoveComments(arr_ptr, count_line);

    for (int i = 0; i < *count_line; ++i)
        printf("{%s}", arr_ptr[i]);
    printf("\n");    

    return arr_ptr;
}


void RemoveComments(char **arr_ptr, int *count_line)            // why is this here?
{
    for (int i = 0; i < *count_line; ++i)
    {
        char* sharp = strchr(arr_ptr[i], '#');
        if (sharp != NULL) *sharp = '\0';
    }
}


token_t *NewToken(type_t type, const char* start, int length, int line, int col)
{
    token_t *new_token = (token_t*)calloc(1, sizeof(token_t));
    if (IS_BAD_PTR(new_token)) return NULL;

    new_token->type   = type;
    new_token->start  = start;
    new_token->length = length;
    new_token->line   = line;
    new_token->col    = col;

    return new_token;
}


tokenErr_t FreeToken(token_t *token)
{
    ON_DEBUG( if (IS_BAD_PTR(token)) return TOKEN_ERROR; )

    token->type   = TOKEN_NONE;
    token->start  = NULL;
    token->length = 0;
    token->line   = 0;
    token->col    = 0;  
    free(token);  

    return TOKEN_SUCCESS;
}