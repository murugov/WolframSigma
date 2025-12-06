#ifndef PARSER_H
#define PARSER_H

#include "tree.h"
#include "token.h"
#include "lexer.h"
#include "DSL.h"

#define MAX_NUM_VAR 4

struct parser_t
{
    lexer_t* lexer;
    token_t* cur_token;
    token_t* prev_token;
};

parser_t* parserCtor(lexer_t* lexer);
void parserDtor(parser_t* parser);
void FreeLines(char **arr_ptr, int count_lines);

node_t* ParseGeneral(parser_t* parser);
node_t* ParseExpression(parser_t* parser);
node_t* ParseTerm(parser_t* parser);
node_t* ParseFactor(parser_t* parser);
node_t* ParsePrimary(parser_t* parser);
node_t* ParseFunctionCall(parser_t* parser);
node_t* ParseVariable(parser_t* parser);
node_t* ParseNumber(parser_t* parser);

void AdvanceToken(parser_t* parser);
int MatchToken(parser_t* parser, type_t type);
int CheckType(parser_t* parser, type_t type);
token_t* ConsumeToken(parser_t* parser, type_t type, const char* error_msg);
void PrintError(parser_t* parser, token_t* token, const char* message);

#define IS_USED_VARS(name, node)  do { \
                                hash_t name_hash = HashStr(name); \
                                for (int i = 0; i < MAX_NUM_VAR; ++i) \
                                { \
                                    if (name_hash == variables[i].hash) \
                                    { \
                                        variables[i].is_used = true; \
                                        return node; \
                                    } \
                                } \
                            } while(0)

#endif