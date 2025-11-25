#include "wolfram.h"
#include "OpInstrSet.cpp"


double CalcExpression(node_t *node)
{
    if (node == NULL) return NAN;
    
    switch (node->type) 
    {
        case ARG_OP:
        {
            double left_val  = CalcExpression(node->left);
            double right_val = CalcExpression(node->right);

            if (isnan(left_val) || isnan(right_val)) return NAN;

            hash_t op_hash = HashStr(node->item.op);
            size_t index   = 0;

            if (HashSearch(op_hash, &index) == TREE_SUCCESS)
            {
                calc_context context = {left_val, right_val};
                return op_instr_set[index].calc(&context);
            }

            return NAN;
        }
        case ARG_VAR:
        {
            hash_t node_hash = HashStr(node->item.var);
            for (int i = 0; i < MAX_NUM_VAR; ++i)
            {
                if (variables[i].hash == node_hash)
                    return variables[i].value;
            }

            return NAN;
        }
        case ARG_NUM:
            return node->item.num;

        default:
            return NAN;
    }
}