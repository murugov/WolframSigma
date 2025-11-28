#include "wolfram.h"
#include "DSL.h"
#include "OpInstrSet.cpp"

var_t variables[MAX_NUM_VAR] = //stack как вектор чтобы можно было for
{
    {119, "w", 0, false},
    {120, "x", 0, false},
    {121, "y", 0, false},
    {122, "z", 0, false}
};


WolfErr_t WolfCtor(tree_t **tree)
{
    *tree = (tree_t*)calloc(1, sizeof(tree_t));
    if (IS_BAD_PTR(tree)) return WOLF_ERROR;

    (*tree)->root = NULL;
    (*tree)->size = 0;
    (*tree)->capacity = 0;

    TREE_INIT((*tree));

    return WOLF_SUCCESS;
}


WolfErr_t WolfDtor(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) {LOG(ERROR, "AKIN_BAD_TREE_PTR"); return WOLF_ERROR; } )

    FreeNodes(tree->root);
    return WOLF_SUCCESS;
}


void EnterVar()
{
    for (int i = 0; i < MAX_NUM_VAR; ++i)
    {
        if (variables[i].is_used)
        {
            printf("Enter the value of the %s: ", variables[i].name);
            scanf("%lg", &(variables[i].value));
        }
    }
}


node_t *DerivativeNode(node_t *node, hash_t hash_indep_var)
{
    if (node == NULL) return NULL;
    
    node_t *tmp = NULL;

    switch (node->type)
    {
        case ARG_OP:
        {   
            hash_t op_hash = HashStr(node->item.op);
            size_t index   = 0;

            if (HashSearch(op_hash, &index) == TREE_SUCCESS)
            {
                diff_context context = {node->left, node->right, hash_indep_var};
                tmp = op_instr_set[index].diff(&context);
                set_parents(tmp, NULL);
                return tmp;
            }
            
            return NULL;
        }
        case ARG_VAR:
        {
            hash_t hash_var = HashStr(node->item.var);
            
            if (hash_var == hash_indep_var)
                return NUM_(1);
            
            return NUM_(0);
        }
        case ARG_NUM:
            return NUM_(0);
            
        default:
            return NULL;
    }
}


node_t *CopyNode(node_t *node)
{
    if (node == NULL) return NULL;

    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    if (IS_BAD_PTR(new_node)) { free(new_node); return NULL; }

    new_node->type = node->type;
    new_node->parent = NULL;
    new_node->item = node->item;

    new_node->left  = CopyNode(node->left);
    new_node->right = CopyNode(node->right);

    return new_node;
}


void set_parents(node_t *node, node_t *parent)
{
    if (node == NULL) return;
    
    node->parent = parent;
    set_parents(node->left, node);
    set_parents(node->right, node);
}