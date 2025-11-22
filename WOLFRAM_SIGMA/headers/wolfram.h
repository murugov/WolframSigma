#ifndef WOLFRAM_H
#define WOLFRAM_H

#include <math.h>
#include "tree.h"
#include "colors.h"
#include "Speak.h"


enum WolfErr_t
{
    WOLF_SUCCESS = 0,
    WOLF_ERROR   = 1
};

#include "WolfOp.h"

WolfErr_t WolfCtor(tree_t **tree);
WolfErr_t WolfDtor(tree_t *tree);

node_t* NodeReader(char* cur_pos, char** next_pos, node_t *parent);
int NameNodeReader(char* cur_pos);
WolfErr_t DataReader(const char *src, tree_t *tree);
WolfErr_t PreOrder(FILE *data, const node_t *node);
WolfErr_t DataUpdate(const char *src, tree_t *tree);


WolfErr_t GenHTML();
WolfErr_t GenGraphs(tree_t *tree, const char *func);
WolfErr_t GenDot(FILE *src, tree_t *tree, const char *func);

#endif