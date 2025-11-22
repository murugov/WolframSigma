#include "tree.h"

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


ArgTypes DetType(char* str)
{
    char tmp[8] = {0};
    if (sscanf(str, "%lg", tmp)) return ARG_NUM;

    hash_t hash = HashStr(str);
    size_t index = 0;
    if (HashSearch(hash, &index) == TREE_SUCCESS) return ARG_OP;

    printf("index: %zu\n", index);
    return ARG_VAR;
}

node_t* NewNode(char* item, ArgTypes type)
{
    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    if (IS_BAD_PTR(new_node)) return NULL;
    
    switch (type)
    {
    case ARG_OP:
        new_node->item.op = atoi(item);
        break;
    case ARG_NUM:
        new_node->item.num = atof(item);
        break;
    case ARG_VAR:
        new_node->item.var = strdup(item);
        break;    
    
    default:
        free(new_node);
        return NULL;
    }

    new_node->parent = NULL;
    new_node->left   = NULL;
    new_node->right  = NULL;
    return new_node;
}


TreeErr_t InsrtLeaf(tree_t *tree, arg_t item)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return TREE_ERROR; )

    // if (tree->root == NULL)
    // { 
    //     tree->root = NewNode(item); 
    //     tree->root->parent = NULL; 
    //     tree->size++; 
    //     ON_DEBUG( LOG(INFO, "Created root: %p", tree->root); )
    //     return TREE_SUCCESS; 
    // }
    // ON_DEBUG( else if (IS_BAD_PTR(tree->root)) return TREE_ERROR; )

    // node_t* current = tree->root;
    // node_t* parent  = NULL;

    // while (current != NULL)
    // {
    //     ON_DEBUG( if (IS_BAD_PTR(current)) return TREE_ERROR; )
    //     parent = current;
    //     if (item < current->item) 
    //         current = current->left;
    //     else if (item >= current->item) 
    //         current = current->right;
    // }

    // node_t* new_node = NewNode(item);
    // if (new_node == NULL) return TREE_ERROR;
    
    // new_node->parent = parent;

    // if (item < parent->item)
    //     parent->left = new_node;
    // else
    //     parent->right = new_node;
        
    // tree->size++;
    
    return TREE_SUCCESS;
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

    if (!IS_BAD_PTR(node->left)) FreeNodes(node->left);
    if (!IS_BAD_PTR(node->right)) FreeNodes(node->right);
    free(node);

    return TREE_SUCCESS;
}