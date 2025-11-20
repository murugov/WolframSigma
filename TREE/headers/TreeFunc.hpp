#include "tree.h"


template <typename treeElem_T>
TreeErr_t TreeInit (tree_t<treeElem_T> *tree, const char *name, const char *file, const char *func, size_t line)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) { LOG(ERROR, ERR_MSG_FORMAT("TREE_BAD_TREE_PTR"), ERR_MSG_PARAMS); return TREE_ERROR; } )
    
    (*tree).id.name = name + 1;
    (*tree).id.file = file;
    (*tree).id.func = func;
    (*tree).id.line = line;

    ON_DEBUG( LOG(DEBUG, "Created id for tree \"%s\"", tree->id.name); )
    return TREE_SUCCESS;
}


template <typename treeElem_T>
TreeErr_t TreeCtor(tree_t<treeElem_T> *tree)
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


template <typename treeElem_T>
node_t<treeElem_T>* NewNode (treeElem_T item)
{
    node_t<treeElem_T> *new_node = (node_t<treeElem_T>*)calloc(1, sizeof(node_t<treeElem_T>));
    if (IS_BAD_PTR(new_node)) return NULL;
    
    new_node->item   = item;
    new_node->parent = NULL;
    new_node->left   = NULL;
    new_node->right  = NULL;
    return new_node;
}


template <typename treeElem_T>
TreeErr_t InsrtLeaf(tree_t<treeElem_T> *tree, treeElem_T item)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return TREE_ERROR; )

    if (tree->root == NULL)
    { 
        tree->root = NewNode(item); 
        tree->root->parent = NULL; 
        tree->size++; 
        ON_DEBUG( LOG(INFO, "Created root: %p", tree->root); )
        return TREE_SUCCESS; 
    }
    ON_DEBUG( else if (IS_BAD_PTR(tree->root)) return TREE_ERROR; )

    node_t<treeElem_T>* current = tree->root;
    node_t<treeElem_T>* parent  = NULL;

    while (current != NULL)
    {
        ON_DEBUG( if (IS_BAD_PTR(current)) return TREE_ERROR; )
        parent = current;
        if (item < current->item) 
            current = current->left;
        else if (item >= current->item) 
            current = current->right;
    }

    node_t<treeElem_T>* new_node = NewNode(item);
    if (new_node == NULL) return TREE_ERROR;
    
    new_node->parent = parent;

    if (item < parent->item)
        parent->left = new_node;
    else
        parent->right = new_node;
        
    tree->size++;
    
    return TREE_SUCCESS;
}


template <typename treeElem_T>
TreeErr_t TreeDtor(tree_t<treeElem_T> *tree)
{
    ON_DEBUG(
        if (IS_BAD_PTR(tree))       return TREE_ERROR;
        if (IS_BAD_PTR(tree->root)) return TREE_ERROR;
    )
    
    FreeNodes(tree->root);
    return TREE_SUCCESS;
}


template <typename treeElem_T>
TreeErr_t FreeNodes(node_t<treeElem_T> *node)
{
    ON_DEBUG( if (IS_BAD_PTR(node)) return TREE_ERROR; )

    if (!IS_BAD_PTR(node->left)) FreeNodes(node->left);
    if (!IS_BAD_PTR(node->right)) FreeNodes(node->right);
    free(node);

    return TREE_SUCCESS;
}