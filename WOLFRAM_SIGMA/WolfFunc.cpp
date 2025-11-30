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


node_t * NDerivativeNode(node_t *node, hash_t hash_indep_var, int count)
{
    if (count == 0) return CopyNode(node);

    node_t *current = DerivativeNode(CopyNode(node), hash_indep_var);
    
    for (int i = 1; i < count; ++i)
        current = DerivativeNode(current, hash_indep_var);
    
    return current;
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


void TaylorSeries(tree_t *tree, const char* indep_var, int point, int order)
{
    tree_t *taylor_tree = {0};
    WolfCtor(&taylor_tree);
    
    node_t *series_sum = NUM_(0);
    hash_t hash_indep_var = HashStr(indep_var);
    node_t *x0 = NUM_(point);
    
    for (int n = 0; n <= order; ++n)
    {
        node_t *derivative_at_point = NULL;
        
        if (n == 0)
        {
            derivative_at_point = Substitute_x0(CopyNode(tree->root), hash_indep_var, x0);
        }
        else
        {
            node_t *nth_derivative = NDerivativeNode(CopyNode(tree->root), hash_indep_var, n);
            derivative_at_point = Substitute_x0(nth_derivative, hash_indep_var, x0);
        }
        
        node_t *coeff = DIV_(derivative_at_point, NUM_(Factorial(n)));
        
        node_t *x_var = NewNode(ARG_VAR, indep_var, NULL, NULL);
        node_t *x_minus_x0 = SUB_(x_var, CopyNode(x0));
        node_t *power_term = POW_(x_minus_x0, NUM_(n));
        
        node_t *new_term = MUL_(coeff, power_term);
        
        series_sum = ADD_(series_sum, new_term);
    }
        
    taylor_tree->root = series_sum;
    SimplifyTree(taylor_tree);

    GenGraphs(taylor_tree, __func__);
    TreeToLatex(taylor_tree, "WOLFRAM_SIGMA/ReportFiles/LatexDump.tex");
    WolfDtor(taylor_tree);
}


node_t* Substitute_x0(node_t *node, hash_t var_hash, node_t *value)
{
    if (IS_BAD_PTR(node)) return NULL;
    
    if (node->type == ARG_VAR && HashStr(node->item.var) == var_hash)
    {
        return CopyNode(value);
    }
    
    node_t *new_node = CopyNode(node);
    new_node->left  = Substitute_x0(node->left,  var_hash, value);
    new_node->right = Substitute_x0(node->right, var_hash, value);

    return new_node;
}