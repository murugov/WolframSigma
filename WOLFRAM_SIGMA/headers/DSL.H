#ifndef DSL_H
#define DSL_H

#define dL DerivativeNode(op_params->node_left, op_params->hash_indep_var)
#define dR DerivativeNode(op_params->node_right, op_params->hash_indep_var)
#define L  CopyNode(op_params->node_left)
#define R  CopyNode(op_params->node_right)

#define NUM_(num) NewNode(ARG_NUM, #num, NULL, NULL)

// #define COMP_FUNC_() 



#define ADD_(node_left, node_right) NewNode(ARG_OP, "+", node_left, node_right)
#define SUB_(node_left, node_right) NewNode(ARG_OP, "-", node_left, node_right)
#define MUL_(node_left, node_right) NewNode(ARG_OP, "*", node_left, node_right)
#define DIV_(node_left, node_right) NewNode(ARG_OP, "/", node_left, node_right)
#define POW_(node_left, node_right) NewNode(ARG_OP, "^", node_left, node_right)



#define EXP_(node_left) NewNode(ARG_OP, "e", node_left, NULL)
#define LG_(node_left)  NewNode(ARG_OP, "lg", node_left, NULL)
#define LOG_(node_left, node_right) NewNode(ARG_OP, "log", node_left, node_right)



#define SIN_(node_left) NewNode(ARG_OP, "sin", node_left, NULL)
#define COS_(node_left) NewNode(ARG_OP, "cos", node_left, NULL)
#define TAN_(node_left) NewNode(ARG_OP, "tan", node_left, NULL)
#define COT_(node_left) NewNode(ARG_OP, "cot", node_left, NULL)



#define COSH_(node_left) NewNode(ARG_OP, "cosh", node_left, NULL)
#define SINH_(node_left) NewNode(ARG_OP, "sinh", node_left, NULL)
#define TANH_(node_left) NewNode(ARG_OP, "tanh", node_left, NULL)
#define COTH_(node_left) NewNode(ARG_OP, "coth", node_left, NULL)



#define ARCSIN_(node_left) NewNode(ARG_OP, "arcsin", node_left, NULL)
#define ARCCOS_(node_left) NewNode(ARG_OP, "arccos", node_left, NULL)
#define ARCTAN_(node_left) NewNode(ARG_OP, "arctan", node_left, NULL)
#define ARCCOT_(node_left) NewNode(ARG_OP, "arccot", node_left, NULL)



#define ARCSINH_(node_left) NewNode(ARG_OP, "arcsinh", node_left, NULL)
#define ARCCOSH_(node_left) NewNode(ARG_OP, "arccosh", node_left, NULL)
#define ARCTANH_(node_left) NewNode(ARG_OP, "arctanh", node_left, NULL)
#define ARCCOTH_(node_left) NewNode(ARG_OP, "arccoth", node_left, NULL)



#endif