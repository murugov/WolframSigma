#include "HashStr.hpp"


hash_t HashStr(const char *line)
{    
    if (IS_BAD_PTR(line)) return 0;
    
    hash_t new_hash = 0;
    const char* ptr = line;
    
    while (*ptr != '\0' && *ptr != '(' && *ptr != ' ' && *ptr != '\t')
    {
        new_hash = (new_hash << 5) - new_hash + (hash_t)(*ptr);
        ptr++;
    }

    return (*ptr == '(' && new_hash == 0) ? (hash_t)('(') : new_hash;
}