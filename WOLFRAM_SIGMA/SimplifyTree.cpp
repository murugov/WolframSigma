#include "wolfram.h"
#include "OpInstrSet.cpp"
#include <math.h>

static int number_changes = 0;


void SimplifyTree(tree_t* tree)
{
    int number_changes_before = number_changes;

    do {
        number_changes_before = number_changes;
        ConstFold(tree, tree->root);
        RemoveNeutralElem(tree, tree->root);
    } while (number_changes_before != number_changes);
}


double ConstFold(tree_t* tree, node_t *node)
{
    if (node == NULL) return NAN;
    
    switch (node->type) 
    {
        case ARG_OP:
        {
            double left_val  = ConstFold(tree, node->left);
            double right_val = ConstFold(tree, node->right);

            hash_t op_hash = HashStr(node->item.op);
            size_t index   = 0;
            
            if ((op_instr_set[index].num_args == 2 && isnan(left_val)) || isnan(right_val)) return NAN;
            
            if (HashSearch(op_hash, &index) == TREE_SUCCESS)
            {
                calc_context context = {left_val, right_val};
                double result = op_instr_set[index].calc(&context);
                ReplaceNode(tree, node, NUM_(result));
                return result;
            }

            return NAN;
        }
        case ARG_VAR:
            return NAN;

        case ARG_NUM:
            return node->item.num;

        default:
            return NAN;
    }
}


node_t *RemoveNeutralElem(tree_t* tree, node_t *node)
{
    if (node == NULL) return NULL;
    
    node->left  = RemoveNeutralElem(tree, node->left);
    node->right = RemoveNeutralElem(tree, node->right);
    
    if (node->type != ARG_OP) return node;
    
    hash_t op_hash = HashStr(node->item.op);
    
    int left_is_num  = (node->left  != NULL && node->left->type  == ARG_NUM);
    int right_is_num = (node->right != NULL && node->right->type == ARG_NUM);
    
    if (op_hash == HASH_MUL)
    {
        if (left_is_num && is_zero(node->left->item.num - 1.0))
        {
            return SwapParentAndChild(tree, node, node->right);
        }
        else if (right_is_num && is_zero(node->right->item.num - 1.0))
        {
            return SwapParentAndChild(tree, node, node->left);
        }
        else if ((left_is_num  && is_zero(node->left->item.num)) || 
                 (right_is_num && is_zero(node->right->item.num)))
        {
            node_t* zero_node = NUM_(0.0);
            ReplaceNode(tree, node, zero_node);
            return zero_node;
        }
    }

    if (op_hash == HASH_DIV)
    {
        if (right_is_num && is_zero(node->right->item.num - 1.0))
        {
            return SwapParentAndChild(tree, node, node->left);
        }
    }

    else if (op_hash == HASH_ADD)
    {
        if (left_is_num && is_zero(node->left->item.num))
        {
            return SwapParentAndChild(tree, node, node->right);
        }
        else if (right_is_num && is_zero(node->right->item.num))
        {
            return SwapParentAndChild(tree, node, node->left);
        }
    }

    else if (op_hash == HASH_SUB)
    {
        if (left_is_num && is_zero(node->left->item.num))
        {
            return MUL_(SwapParentAndChild(tree, node, node->right), NUM_(-1.0));
        }
        else if (right_is_num && is_zero(node->right->item.num))
        {
            return SwapParentAndChild(tree, node, node->left);
        }
    }

    else if (op_hash == HASH_POW)
    {
        if (right_is_num && is_zero(node->right->item.num))
        {
            node_t* one_node = NUM_(1.0);
            ReplaceNode(tree, node, one_node);
            return one_node;
        }
        else if (left_is_num && is_zero(node->left->item.num - 1.0))
        {
            node_t* one_node = NUM_(1.0);
            ReplaceNode(tree, node, one_node);
            return one_node;
        }
        else if (right_is_num && is_zero(node->right->item.num - 1.0))
        {
            return SwapParentAndChild(tree, node, node->left);
        }
    }
    
    return node; 
}


void ReplaceNode(tree_t *tree, node_t *old_node, node_t *new_node)
{
    if (IS_BAD_PTR(old_node) || IS_BAD_PTR(new_node)) return;
    
    if (old_node->parent == NULL)
    {
        tree->root = new_node;
        new_node->parent = NULL;
    }
    else
    {
        new_node->parent = old_node->parent;
        
        if (old_node->parent->left == old_node)
            old_node->parent->left = new_node;
        else
            old_node->parent->right = new_node;
    }
    
    FreeNodes(old_node);
    number_changes++;
}


node_t *SwapParentAndChild(tree_t *tree, node_t *parent, node_t *child)
{
    ON_DEBUG( if (IS_BAD_PTR(tree) || IS_BAD_PTR(parent) || IS_BAD_PTR(child)) return NULL; )

    node_t *new_node = CopyNode(child);
    ReplaceNode(tree, parent, new_node);
    return new_node;
}