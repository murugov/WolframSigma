#ifndef WOLFRAM_H
#define WOLFRAM_H

#include "math_func.h"
#include "tree.h"
#include "is_zero.h"
#include "Factorial.h"
#include "colors.h"
#include "DSL.h"

#include "WolfOp.h"

extern FILE *file_latex;

#define MAX_NUM_VAR 4

struct var_t
{
    hash_t hash;
    char   name[8];
    double value;
    bool   is_used;
};

extern var_t variables[MAX_NUM_VAR];


enum WolfErr_t
{
    WOLF_SUCCESS = 0,
    WOLF_ERROR   = 1
};


WolfErr_t WolfCtor(tree_t **tree);
WolfErr_t WolfDtor(tree_t *tree);

void EnterVar();
node_t *DerivativeNode(node_t *node, hash_t hash_indep_var);
void set_parents(node_t *node, node_t *parent);
node_t *CopyNode(node_t *node);

node_t * NDerivativeNode(node_t *node, hash_t hash_indep_var, int count);

void TaylorSeries(tree_t *tree, const char* indep_var, double point, int order);
node_t* Substitute_x0(node_t *node, hash_t var_hash, node_t *value);

void SimplifyTree(tree_t* tree);
double ConstFold(tree_t* tree, node_t *node);
node_t *RemoveNeutralElem(tree_t* tree, node_t *node);
void ReplaceNode(tree_t *tree, node_t *old_node, node_t *new_node);
node_t *SwapParentAndChild(tree_t *tree, node_t *parent, node_t *child);

double CalcExpression(node_t *node);

WolfErr_t DataReader(const char *src, tree_t *tree);

WolfErr_t GenHTML();
WolfErr_t GenGraphs(tree_t *tree, const char *func);
WolfErr_t GenDot(FILE *src, tree_t *tree, const char *func);

WolfErr_t LatexFileOpener(const char* path);
WolfErr_t LatexFileCloser();

void TreeToLatex(node_t *node);
void NodeToLatex(node_t *node, node_t *parent = NULL, bool is_left = true);


#define SKIP_SPACES(ptr) while (isspace((int)*ptr)) ptr++

#define OP_(op)   NewNode(ARG_OP, valVAR(op), NULL, NULL)
#define VAR_(var) NewNode(ARG_VAR, valVAR(var), NULL, NULL)
#define NUM_(num) NewNode(ARG_NUM, num, NULL, NULL)

#define LATEX(node) do{ \
                        fprintf(file_latex, "\\[\n"); \
                        NodeToLatex(node); \
                        fprintf(file_latex, "\n\\]\n\n"); \
                    } while(0)

#endif