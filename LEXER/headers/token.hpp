#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "stack.hpp"
#include "TXTreader.hpp"

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
    TOKEN_DEF      = 0x0D,
    // operators
    TOKEN_ADD      = 0x0E,
    TOKEN_SUB      = 0x0F,
    TOKEN_MUL      = 0x10,
    TOKEN_DIV      = 0x11,
    TOKEN_POW      = 0x12,
    TOKEN_EQ       = 0x13,
    TOKEN_EQEQ     = 0x14,
    // brackets and separators
    TOKEN_LPAREN   = 0x15,
    TOKEN_RPAREN   = 0x16,
    TOKEN_LBRACKET = 0x17,
    TOKEN_RBRACKET = 0x18,
    TOKEN_LBRACE   = 0x19,
    TOKEN_RBRACE   = 0x1A,
    TOKEN_COMMA    = 0x1B,
    TOKEN_SEMICOLON= 0x1C,
    TOKEN_COLON    = 0x1D,
    // special symbols
    TOKEN_COMMENT  = 0x1E,
    TOKEN_EOF      = 0x1F,
    TOKEN_DOLLAR   = 0x20,
    TOKEN_UNDEF    = 0x21
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