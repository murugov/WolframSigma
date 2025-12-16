#include "tree.hpp"


TreeErr_t TreeInit(tree_t *tree, const char *name, const char *file, const char *func, size_t line)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) { LOG(ERROR, ERR_MSG_FORMAT("TREE_BAD_TREE_PTR"), ERR_MSG_PARAMS); return TREE_ERROR; } )
    
    (*tree).id.name = name;
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


node_t *NewNode(type_t type, val item, node_t *left, node_t *right)
{
    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    if (IS_BAD_PTR(new_node)) { return NULL; }

    switch (type)
    {
        case ARG_NUM:
            new_node->item.num = item.num;
            break;
        case ARG_OP:
            new_node->item.op = item.op;
            break;
        case ARG_VAR:
            new_node->item.var = item.var;
            break;    
        case ARG_FUNC:
            new_node->item.func = item.func;
            break;   
        default:
            free(new_node);
            return NULL;
    }

    new_node->type   = type;
    new_node->parent = NULL;
    new_node->left   = left;
    new_node->right  = right;
    return new_node;
}

val valVAR(const char* ptr, int len)  { val v; v.var  = strndup(ptr, (size_t)len); return v; }
val valFUNC(const char* ptr, int len) { val f; f.func = strndup(ptr, (size_t)len); return f; }


void set_parents(node_t *node, node_t *parent)
{
    if (node == NULL) return;
    
    node->parent = parent;
    set_parents(node->left, node);
    set_parents(node->right, node);
}


TreeErr_t TreeDtor(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree) || IS_BAD_PTR(tree->root)) return TREE_ERROR; )
    
    FreeNodes(tree->root);
    return TREE_SUCCESS;
}


TreeErr_t FreeNodes(node_t *node)
{
    if (IS_BAD_PTR(node)) return TREE_ERROR;

    if (!IS_BAD_PTR(node->left))  FreeNodes(node->left);
    if (!IS_BAD_PTR(node->right)) FreeNodes(node->right);
    
    switch (node->type)
    {
        case ARG_VAR:
            if (!IS_BAD_PTR(node->item.var))
            {
                free(node->item.var);
                node->item.var = NULL;
            }
            break;
            
        case ARG_FUNC:
            if (!IS_BAD_PTR(node->item.func))
            {
                free(node->item.func);
                node->item.func = NULL;
            }
            break;
            
        case ARG_NUM:
        case ARG_OP:
            break;
            
        default:
            ON_DEBUG( LOG(ERROR, "Unknown node type: %d", node->type); )
            break;
    }
    
    free(node);
    
    return TREE_SUCCESS;
}