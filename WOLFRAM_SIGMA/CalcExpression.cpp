#include "wolfram.hpp"
#include "OpInstrSet.cpp"


double CalcExpression(node_t *node)
{
    if (node == NULL) return NAN;
    
    switch (node->type) 
    {
        case ARG_OP:
        {
            size_t index   = 0;
            if (HashSearch(node->item.op, &index) == WOLF_SUCCESS)
            {
                double left_val = NAN, right_val = NAN;
                
                if (op_instr_set[index].num_args >= 1) 
                {
                    right_val = CalcExpression(node->right);
                    if (isnan(right_val)) return NAN;
                }
                
                if (op_instr_set[index].num_args == 2) 
                {
                    left_val = CalcExpression(node->left);
                    if (isnan(left_val)) return NAN;
                }
                
                calc_context context = {left_val, right_val};
                return op_instr_set[index].calc(&context);
            }

            return NAN;
        }
        case ARG_VAR:
        {
            var_t var = {};
            var.name = node->item.var;
            var_t *var_ptr = &var;

            var_t *found_var = htFind(variables, &(var_ptr), htVarToStr);
            if (!IS_BAD_PTR(found_var)) return found_var->value;

            return NAN;
        }
        case ARG_NUM:
            return node->item.num;

        default:
            return NAN;
    }
}