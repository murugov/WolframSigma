#ifndef LEXER_H
#define LEXER_H

#include "wolfram.h"


node_t *GetGeneral(const char **lex_pos);
node_t *GetExpress(const char **lex_pos);
node_t *GetTerm(const char **lex_pos);
node_t *GetPow(const char **lex_pos);
node_t *GetFunc(const char **lex_pos);
node_t *GetName(const char **lex_pos);
node_t *GetParen(const char **lex_pos);
node_t *GetNum(const char **lex_pos);
bool isValidName(const char* name);


#define IS_USED_VARS(name)  do { \
                                hash_t name_hash = HashStr(name); \
                                for (int i = 0; i < MAX_NUM_VAR; ++i) \
                                { \
                                    if (name_hash == variables[i].hash) \
                                    { \
                                        variables[i].is_used = true; \
                                        return node_name; \
                                    } \
                                } \
                            } while(0)
#endif