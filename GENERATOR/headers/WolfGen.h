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

#define PATH_TO_SRC_FILE     "GENERATOR/src/func.txt"
#define PATH_TO_WOLF_OP_H    "GENERATOR/ReportFiles/WolfOp.h"
#define PATH_TO_OP_INSTR_SET "GENERATOR/ReportFiles/OpInstrSet.cpp"

#define MAX_LEN_NAME_FUNC 16

enum GenErr_t
{
    GEN_SUCCESS = 0,
    GEN_ERROR   = 1
};

struct funcInfo
{
    char name[MAX_LEN_NAME_FUNC];
    char op[MAX_LEN_NAME_FUNC];
};

struct op_instr_t
{
    char name[32];
    char op[8];
    hash_t hash;
    int num_args; 
};

void RemoveComments(char** arr_cmd, size_t *count_line);

GenErr_t GenWolfOp(FILE *WolfOpFile, char **arr_ptr, size_t count_line);
GenErr_t GenOpInstrSet(FILE *OpInstrSetFile, char **arr_ptr, size_t count_line);

int CmpOpInstrSetByHash(const void *a, const void *b);
int CmpByHash(const void *a, const void *b);

#endif