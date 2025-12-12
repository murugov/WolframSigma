#ifndef HASH_STR_HPP
#define HASH_STR_HPP

#include <stdio.h>

#define MAX_LEN_CMD 16
typedef size_t hash_t;

hash_t HashStr(const char *line);

#endif