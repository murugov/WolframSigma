#ifndef WOLFRAM_H
#define WOLFRAM_H

#include "math_func.h"
#include "tree.h"
#include "is_zero.h"
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
void set_parents(node_t *node, node_t *parent);
node_t *CopyNode(node_t *node);

void SimplifyTree(tree_t* tree);
double ConstFold(node_t *node);
double RemoveNeutralElem(node_t *node);
void ReplaceNode(node_t *old_node, node_t *new_node);
void ReplaceNodeWithChild(node_t *parent, node_t *child);

double CalcExpression(node_t *node);

node_t* NodeReader(char* cur_pos, char** next_pos, node_t *parent);
int NameNodeReader(char* cur_pos);
WolfErr_t DataReader(const char *src, tree_t *tree);

WolfErr_t GenHTML();
WolfErr_t GenGraphs(tree_t *tree, const char *func);
WolfErr_t GenDot(FILE *src, tree_t *tree, const char *func);

#define NUM_(num) NewNode(ARG_NUM, #num, NULL, NULL)

#endif