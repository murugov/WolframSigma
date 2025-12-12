#include "tree.hpp"
#include "wolfram.hpp"
#include "OpInstrSet.cpp"

TreeErr_t TreeInit (tree_t *tree, const char *name, const char *file, const char *func, size_t line)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) { LOG(ERROR, ERR_MSG_FORMAT("TREE_BAD_TREE_PTR"), ERR_MSG_PARAMS); return TREE_ERROR; } )
    
    (*tree).id.name = name + 1;
    (*tree).id.file = file;
    (*tree).id.func = func;
    (*tree).id.line = line;

    ON_DEBUG( LOG(DEBUG, "Created id for tree \"%s\"", tree->id.name); )
    return TREE_SUCCESS;
}


TreeErr_t TreeCtor(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) { LOG(ERROR, ERR_MSG_FORMAT("TREE_BAD_TREE_PTR"), ERR_MSG_PARAMS); return TREE_ERROR; } )

    tree->canary_1 = TREE_CANARY_1;
    tree->root     = NULL;
    tree->size     = 0;
    tree->capacity = 0;
    tree->error    = TREE_NO_ERRORS;
    tree->canary_2 = TREE_CANARY_2;

    return TREE_SUCCESS;
}


ArgTypes DetType(char* str)                 // needs to be moved to WOLFRAM_SIGMA
{
    hash_t hash = HashStr(str);
    for (int i = 0; i < MAX_NUM_VAR; ++i)
    {
        if (variables[i].hash == hash)
        {
            variables[i].is_used = true;
            return ARG_VAR; 
        }
    }

    size_t index = 0;
    if (HashSearch(hash, &index) == TREE_SUCCESS) return ARG_OP;

    return ARG_NUM;
}


TreeErr_t HashSearch(hash_t hash, size_t *index)
{
    ON_DEBUG( if (IS_BAD_PTR(index)) return TREE_ERROR; )

    op_t *found = (op_t*)bsearch(&hash, op_instr_set, LEN_INSTR_SET, sizeof(op_instr_set[0]), CmpForBinSearch);
    if (found == NULL)
        return TREE_ERROR;
    
    *index = (size_t)(found - op_instr_set);
    return TREE_SUCCESS;
}


int CmpForBinSearch(const void *a, const void *b)
{
    const hash_t *hash_a = (const hash_t*)a;
    const op_t   *op_b   = (const op_t*)b;
    
    if (*hash_a > op_b->hash)
        return 1;
    if (*hash_a < op_b->hash)
        return -1;
    return 0;
}


node_t *NewNode(ArgTypes type, val item, node_t *left, node_t *right)
{
    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    if (IS_BAD_PTR(new_node)) { free(new_node); return NULL; }

    switch (type)
    {
    case ARG_OP:
        new_node->item.op = strdup(item.op);
        break;
    case ARG_NUM:
        new_node->item.num = item.num;
        break;
    case ARG_VAR:
        new_node->item.var = strdup(item.var);
        break;    
    
    default:
        free(new_node);
        return NULL;
    }

    new_node->type = type;
    new_node->parent = NULL;
    new_node->left   = left;
    new_node->right  = right;
    return new_node;
}

val valOP(const char* s)  { val v; v.op  = const_cast<char*>(s); return v; }
val valVAR(const char* s) { val v; v.var = const_cast<char*>(s); return v; }


void set_parents(node_t *node, node_t *parent)
{
    if (node == NULL) return;
    
    node->parent = parent;
    set_parents(node->left, node);
    set_parents(node->right, node);
}


TreeErr_t TreeDtor(tree_t *tree)
{
    ON_DEBUG(
        if (IS_BAD_PTR(tree))       return TREE_ERROR;
        if (IS_BAD_PTR(tree->root)) return TREE_ERROR;
    )
    
    FreeNodes(tree->root);
    return TREE_SUCCESS;
}


TreeErr_t FreeNodes(node_t *node)
{
    ON_DEBUG( if (IS_BAD_PTR(node)) return TREE_ERROR; )

    if (!IS_BAD_PTR(node->left))  FreeNodes(node->left);
    if (!IS_BAD_PTR(node->right)) FreeNodes(node->right);
    free(node);

    return TREE_SUCCESS;
}