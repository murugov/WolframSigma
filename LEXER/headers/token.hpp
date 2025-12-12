#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
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
    TOKEN_NONE     = 0x00,
    // operands
    TOKEN_NUM      = 0x01,
    TOKEN_VAR      = 0x02,
    TOKEN_ID       = 0x03,
    TOKEN_STR      = 0x04,
    // keywords
    TOKEN_FUNC     = 0x05,
    // TOKEN_DEF      = 0x06,
    // TOKEN_PLOT     = 0x07,
    TOKEN_IF       = 0x08,
    TOKEN_ELSE     = 0x09,
    TOKEN_FOR      = 0x0A,
    TOKEN_WHILE    = 0x0B,
    TOKEN_RETURN   = 0x0C,
    // operators
    TOKEN_ADD      = 0x0D,
    TOKEN_SUB      = 0x0E,
    TOKEN_MUL      = 0x0F,
    TOKEN_DIV      = 0x10,
    TOKEN_POW      = 0x11,
    TOKEN_EQ       = 0x12,
    TOKEN_EQEQ     = 0x13,
    // brackets and separators
    TOKEN_LPAREN   = 0x14,
    TOKEN_RPAREN   = 0x15,
    TOKEN_LBRACKET = 0x16,
    TOKEN_RBRACKET = 0x17,
    TOKEN_LBRACE   = 0x18,
    TOKEN_RBRACE   = 0x19,
    TOKEN_COMMA    = 0x1A,
    TOKEN_SEMICOLON= 0x1B,
    TOKEN_COLON    = 0x1C,
    // special symbols
    TOKEN_COMMENT  = 0x1D,
    TOKEN_EOF      = 0x1E,
    TOKEN_DOLLAR   = 0x1F,
    TOKEN_UNDEF    = 0x20
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

token_t *NewToken(type_t type, const char* start, int length, int line, int col);
tokenErr_t FreeToken(token_t *token);

char **DataReader(FILE *SourceFile, char *buffer, int *count_line);
void RemoveComments(char **arr_ptr, int *count_line);

#endif