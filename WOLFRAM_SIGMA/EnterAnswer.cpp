#include "wolfram.h"


WolfErr_t EnterAnswer(char** answer, ssize_t *len, size_t *capacity)
{
    ON_DEBUG( if (IS_BAD_PTR(answer) || IS_BAD_PTR(len) || IS_BAD_PTR(capacity)) return WOLF_ERROR; )

    *len = getline(answer, capacity, stdin);
    if (*len == -1) { perror("Error reading user answer:"); return WOLF_ERROR; }
    (*answer)[*len - 1] = '\0';
    
    for (ssize_t i = 0; i < *len - 1; ++i)
        (*answer)[i] = (char)tolower((*answer)[i]);

    return WOLF_SUCCESS;
}


WolfErr_t YesOrNo(char** answer, ssize_t *len, size_t *capacity)
{
    ON_DEBUG( if (IS_BAD_PTR(answer) || IS_BAD_PTR(len) || IS_BAD_PTR(capacity)) return WOLF_ERROR; )
    
    printf(ANSI_COLOR_BLUE "Enter \"yes\" or \"no\"\n" ANSI_COLOR_RESET);
    EnterAnswer(answer, len, capacity);
    
    while (strncmp(*answer, "yes", 3) != 0 && strncmp(*answer, "no", 2) != 0)
    {
        printf(ANSI_COLOR_RED "Invalid input\n" ANSI_COLOR_RESET);
        EnterAnswer(answer, len, capacity);
    }

    return WOLF_SUCCESS;
}


int NameNodeReader(char* cur_pos)
{
    ON_DEBUG( if (cur_pos == NULL) return 0; )

    int len_name = 0;
    sscanf(cur_pos, "\"%*[^\"]\"%n", &len_name);

    return len_name - 2;
}