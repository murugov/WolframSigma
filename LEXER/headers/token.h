#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "colors.h"
#include "CONFIG.h"
#include "DEBUG_MOD.h"
#include "ERR_FIND_MOD.h"
#include "IsBadPtr.h"
#include "logger.h"
#include "TXTreader.h"
#include "HashStr.h"


enum tokenErr_t
{
    TOKEN_SUCCESS = 0,
    TOKEN_ERROR   = 1
};

enum tokenTypes
{
    // operands
    TOKEN_NUM      = 0x00,
    TOKEN_VAR      = 0x01,
    TOKEN_ID       = 0x02,
    TOKEN_STR      = 0x03,
    // keywords
    TOKEN_FUNC     = 0x04,
    // TOKEN_BUILTIN  = 0x05,
    TOKEN_IF       = 0x06,
    TOKEN_ELSE     = 0x07,
    TOKEN_FOR      = 0x08,
    TOKEN_WHILE    = 0x09,
    TOKEN_RETURN   = 0x0A,
    // operators
    TOKEN_ADD      = 0x0B,
    TOKEN_SUB      = 0x0C,
    TOKEN_MUL      = 0x0D,
    TOKEN_DIV      = 0x0E,
    TOKEN_POW      = 0x0F,
    TOKEN_EQ       = 0x10,
    TOKEN_EQEQ     = 0x11,
    // brackets and separators
    TOKEN_LPAREN   = 0x12,
    TOKEN_RPAREN   = 0x13,
    TOKEN_LBRACKET = 0x14,
    TOKEN_RBRACKET = 0x15,
    TOKEN_LBRACE   = 0x16,
    TOKEN_RBRACE   = 0x17,
    TOKEN_COMMA    = 0x18,
    TOKEN_SEMICOLON= 0x19,
    TOKEN_COLON    = 0x1A,
    // special symbols
    TOKEN_COMMENT  = 0x1B,
    TOKEN_EOF      = 0x1C,
    TOKEN_DOLLAR   = 0x1D,
    TOKEN_UNDEF    = 0x1E
};

typedef tokenTypes type_t;

struct token_t
{
    type_t      type;
    const char* start;
    int         length;
    int         line;
    int         col;
};


char **DataReader(FILE *SourceFile, char *buffer, int *count_line);
void RemoveComments(char **arr_ptr, int *count_line);

#endif