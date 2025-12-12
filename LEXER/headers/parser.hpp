#ifndef PARSER_H
#define PARSER_H

#include "tree.h"
#include "token.hpp"
#include "lexer.hpp"
#include "DSL.h"


void FreeLines(char **arr_ptr, int count_lines);

node_t* ParseGeneral(lexer_t* lexer);
node_t* ParseExpression(lexer_t* lexer);
node_t* ParseTerm(lexer_t* lexer);
node_t* ParseFactor(lexer_t* lexer);
node_t* ParsePrimary(lexer_t* lexer);
node_t* ParseFunc(lexer_t* lexer);
node_t* ParseVariable(lexer_t* lexer);
node_t* ParseNumber(lexer_t* lexer);

int MatchToken(lexer_t* lexer, type_t type);
int CheckType(lexer_t* lexer, type_t type);
token_t* ConsumeToken(lexer_t* lexer, type_t type, const char* error_msg);
void PrintError(lexer_t* lexer, token_t* token, const char* message);

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