#include "wolfram.h"
#include "OpInstrSet.cpp"
#include <math.h>

static int number_changes = 0;


void SimplifyTree(tree_t* tree)
{
    int number_changes_before = number_changes;

    ConstFold(tree);
    RemoveNeutralElem(tree);

    if (number_changes_before == number_changes) return;
}


double ConstFold(tree_t* tree)
{
    if (IS_BAD_PTR(tree) || IS_BAD_PTR(tree->root)) return NAN;
    return ConstFoldNode(tree, tree->root);
}

double ConstFoldNode(tree_t* tree, node_t *node)
{
    if (node == NULL) return NAN;
    
    switch (node->type) 
    {
        case ARG_OP:
        {
            double left_val  = ConstFoldNode(tree, node->left);
            double right_val = ConstFoldNode(tree, node->right);

            if (isnan(left_val) || isnan(right_val)) return NAN;

            hash_t op_hash = HashStr(node->item.op);
            size_t index   = 0;

            if (HashSearch(op_hash, &index) == TREE_SUCCESS)
            {
                calc_context context = {left_val, right_val};
                double result = op_instr_set[index].calc(&context);
                
                node_t *num_node = NUM_(result);
                ReplaceNode(tree, node, num_node);
                number_changes++;
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


double RemoveNeutralElem(tree_t* tree)
{
    if (IS_BAD_PTR(tree) || IS_BAD_PTR(tree->root)) return NAN;
    return RemoveNeutralElemNode(tree, tree->root);
}

double RemoveNeutralElemNode(tree_t* tree, node_t *node)
{
    if (node == NULL) return NAN;
    
    switch (node->type) 
    {
        case ARG_OP:
        {
            double left_val = RemoveNeutralElemNode(tree, node->left);
            double right_val = RemoveNeutralElemNode(tree, node->right);

            hash_t op_hash = HashStr(node->item.op);
            
            if (op_hash == HASH_MUL)
            {
                if (is_zero(left_val - 1.0))
                {
                    ReplaceNodeWithChild(tree, node, node->right);
                    return right_val;
                }
                else if (is_zero(right_val - 1.0))
                {
                    ReplaceNodeWithChild(tree, node, node->left);
                    return left_val;
                }
                else if (is_zero(left_val) || is_zero(right_val))
                {
                    node_t* zero_node = NUM_(0);
                    ReplaceNode(tree, node, zero_node);
                    return 0.0;
                }
            }
            else if (op_hash == HASH_ADD)
            {
                if (is_zero(left_val))
                {
                    ReplaceNodeWithChild(tree, node, node->right);
                    return right_val;
                }
                else if (is_zero(right_val))
                {
                    ReplaceNodeWithChild(tree, node, node->left);
                    return left_val;
                }
            }
            else if (op_hash == HASH_POW)
            {
                if (is_zero(right_val))
                {
                    node_t* one_node = NUM_(1);
                    ReplaceNode(tree, node, one_node);
                    return 1.0;
                }
                else if (is_zero(right_val - 1.0))
                {
                    ReplaceNodeWithChild(tree, node, node->left);
                    return left_val;
                }
                else if (is_zero(left_val - 1.0))
                {
                    node_t* one_node = NUM_(1);
                    ReplaceNode(tree, node, one_node);
                    return 1.0;
                }
            }

            size_t index = 0;
            if (HashSearch(op_hash, &index) == TREE_SUCCESS)
            {
                calc_context context = {left_val, right_val};
                return op_instr_set[index].calc(&context);
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


void ReplaceNode(tree_t *tree, node_t *old_node, node_t *new_node)
{
    if (old_node == NULL || new_node == NULL) return;
    
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


void ReplaceNodeWithChild(tree_t *tree, node_t *parent, node_t *child)
{
    if (parent == NULL || child == NULL) return;
    
    if (parent->parent == NULL)
    {
        tree->root = child;
        child->parent = NULL;
    }
    else
    {
        child->parent = parent->parent;
        
        if (parent->parent->left == parent)
            parent->parent->left = child;
        else
            parent->parent->right = child;
    }
    
    free(parent->item.op);
    free(parent);
    number_changes++;
}