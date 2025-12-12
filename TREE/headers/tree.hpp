#ifndef TREE_HPP
#define TREE_HPP

#include "CONFIG.hpp"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "IsBadPtr.hpp"
#include "HashStr.hpp"
#include "logger.hpp"
#include "stack.hpp"

#define TREE_POISON 0       // improve

typedef int    arg_t;
typedef size_t tree_canary_t;
typedef size_t tree_err_t;
typedef size_t hash_t;

enum ArgTypes
{
    ARG_OP  = 0x00,
    ARG_VAR = 0x01,
    ARG_NUM = 0x02
};

enum TreeErr_t
{
    TREE_SUCCESS = 0,
    TREE_ERROR   = 1
};

enum TreeErrCodes_t
{
    TREE_NO_ERRORS          = 0,
    TREE_BAD_TREE_PTR       = 1 << 0,
    TREE_WRONG_CANARY       = 1 << 1,
    TREE_WRONG_HASH         = 1 << 2,
    TREE_WRONG_SIZE         = 1 << 3,
    TREE_WRONG_CAPACITY     = 1 << 4,
    TREE_WRONG_LEFT_PTR     = 1 << 5,
    TREE_WRONG_RIGHT_PTR    = 1 << 6,
    TREE_REALLOC_FAIL       = 1 << 7,
    TREE_REV_REALLOC_FAIL   = 1 << 8,
    TREE_DATA_OVERFLOW      = 1 << 9,
    TREE_ACCESS_EMPTY_DATA  = 1 << 10,
    TREE_GEN_GRAPH_FAIL     = 1 << 11,
    TREE_GEN_DOT_FAIL       = 1 << 12,
    TREE_GEN_HTML_FAIL      = 1 << 13
};

enum TreeFunc
{
    TREE_DEFUALT = 0x00,
    TREE_CTOR    = 0x01,
    TREE_DTOR    = 0x02,
    TREE_POP     = 0x03,
    TREE_PUSH    = 0x04,
    TREE_REALLOC = 0x05
};

enum TreeCanary
{
    TREE_CANARY_1 = 111,
    TREE_CANARY_2 = 222
};

struct tree_id
{
    const char *name;
    const char *file;
    const char *func;
    size_t     line;
};

union val
{
    double num;
    char*  op;
    char*  var;

    val() : num(0) {}
    val(double n) : num(n) {}
};

val valOP(const char* s);
val valVAR(const char* s);


struct node_t
{
    arg_t  type;
    val    item;
    node_t *parent;
    node_t *left;
    node_t *right;
};

struct tree_t
{
    tree_canary_t canary_1;
    node_t        *root;
    ssize_t       size;
    ssize_t       capacity;
    tree_id       id;
    tree_err_t    error;
    tree_canary_t canary_2;
};


TreeErr_t TreeInit(tree_t *tree, const char *name, const char *file, const char *func, size_t line);
TreeErr_t TreeCtor(tree_t **tree);

node_t *NewNode(ArgTypes type, val item, node_t *left, node_t *right);
void set_parents(node_t *node, node_t *parent);

TreeErr_t FreeNodes(node_t *node);
TreeErr_t TreeDtor(tree_t *tree);


#define TREE_CTOR(tree) TreeInit(tree, #tree, __FILE__, __func__, __LINE__); TreeCtor(&(tree))
#define TREE_DTOR(tree) TreeDtor(tree)

#define OP_(op)   NewNode(ARG_OP, valVAR(op), NULL, NULL)
#define VAR_(var) NewNode(ARG_VAR, valVAR(var), NULL, NULL)
#define NUM_(num) NewNode(ARG_NUM, num, NULL, NULL)

#endif