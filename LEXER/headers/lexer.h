#ifndef LEXER_H
#define LEXER_H

#include "token.h"


enum lexerErr_t
{
    LEX_SUCCESS = 0,
    LEX_ERROR   = 1
};

struct lexer_t
{
    char**      lines;
    int         line_count;
    int         cur_line;
    int         cur_col;
    const char* cur_pos;
    const char* file_name;
};

lexerErr_t LexerInit(lexer_t* lexer, char** lines, int line_count, const char* file_name);
token_t *NewToken(type_t type, const char* start, int length, int line, int col);

lexerErr_t SkipSpaces(lexer_t* lexer);
token_t *NextToken(lexer_t* lexer);
token_t *PeekToken(lexer_t* lexer);

#define IS_END(lexer) (lexer->cur_line >= lexer->line_count)

#endif