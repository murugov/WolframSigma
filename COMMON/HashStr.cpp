#include "HashStr.h"


hash_t HashStr(const char *line)
{    
    hash_t new_hash = 0;
    char cmd[MAX_LEN_CMD] = {0};
    
    if(sscanf(line, "%s", cmd) != 1) return 0;
    
    const char* ptr = cmd;
    while (*ptr != '\0')
    {
        new_hash = (new_hash << 5) - new_hash + (hash_t)(*ptr);
        ptr++;
    }

    return new_hash;
}