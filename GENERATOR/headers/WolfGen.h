#ifndef WOLF_GEN_H
#define WOLF_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "IsBadPtr.h"
#include "TXTreader.h"
#include "HashStr.h"
#include "colors.h"

enum GenErr_t
{
    GEN_SUCCESS = 0,
    GEN_ERROR   = 1
};


void RemoveComments(char** arr_cmd, size_t *count_line);

GenErr_t GenWolfOp(FILE *WolfOpFile, char **arr_ptr, size_t count_line);
// GenErr_t GenOpInstrSet(FILE *OpInstrSetFile, char **arr_ptr, size_t count_line);

// int CmpByHash(const void *a, const void *b);

#define IS_BAD_PTR(ptr) IsBadPtr((void*)ptr)

#endif