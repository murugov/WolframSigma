#ifndef HASH_STR_HPP
#define HASH_STR_HPP

#include "CONFIG.hpp"

#include <stdio.h>
#include "IsBadPtr.hpp"

#define MAX_LEN_CMD 16

hash_t HashStr(const char *line);

#endif