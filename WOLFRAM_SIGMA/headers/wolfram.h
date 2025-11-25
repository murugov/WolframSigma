#ifndef WOLFRAM_H
#define WOLFRAM_H

#include "math_func.h"
#include "tree.h"
#include "colors.h"

#include "WolfOp.h"

#define MAX_NUM_VAR 4

struct var_t
{
    hash_t hash;
    char   name[8];
    double value;
    bool   is_used;
};

extern var_t variables[MAX_NUM_VAR];

WolfErr_t WolfCtor(tree_t **tree);
WolfErr_t WolfDtor(tree_t *tree);

void EnterVar();
node_t *DerivativeNode(node_t *node, hash_t hash_indep_var);
node_t *CopyNode(node_t *node);

node_t* NodeReader(char* cur_pos, char** next_pos, node_t *parent);
int NameNodeReader(char* cur_pos);
WolfErr_t DataReader(const char *src, tree_t *tree);

WolfErr_t GenHTML();
WolfErr_t GenGraphs(tree_t *tree, const char *func);
WolfErr_t GenDot(FILE *src, tree_t *tree, const char *func);

#endif