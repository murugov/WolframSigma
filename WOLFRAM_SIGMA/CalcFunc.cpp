#include "wolfram.h"
#include "DSL.H"


// elementary functions
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double funcADD(op_context *op_params) { return op_params->node_left->item.num + op_params->node_right->item.num; }
node_t *diffADD(op_context *op_params) { return ADD_(dL, dR); }

double funcSUB(op_context *op_params) { return op_params->node_left->item.num - op_params->node_right->item.num; }
node_t *diffSUB(op_context *op_params) { return SUB_(dL, dR); }

double funcMUL(op_context *op_params) { return op_params->node_left->item.num * op_params->node_right->item.num; }
node_t *diffMUL(op_context *op_params) { return ADD_(MUL_(dL, R), MUL_(L, dR)); }

double funcDIV(op_context *op_params) { return op_params->node_left->item.num / op_params->node_right->item.num; } // div by zero!!!
node_t *diffDIV(op_context *op_params) { return DIV_(SUB_(MUL_(dL, R), MUL_(dR, L)), MUL_(R, R)); }

double funcPOW(op_context *op_params) { return pow(op_params->node_left->item.num, op_params->node_right->item.num); } // zero to the power of zero!!!
node_t *diffPOW(op_context *op_params) 
{ 
    node_t *base = op_params->node_left;
    node_t *exponent = op_params->node_right;
    
    if (base->type == ARG_NUM)
        return MUL_(MUL_(LG_(L), POW_(L, R)), dR);

    else if (exponent->type == ARG_NUM)
        return MUL_(MUL_(R, POW_(L, SUB_(R, NUM_(1)))), dL);

    else 
        return MUL_(POW_(L, R), ADD_(MUL_(dR, LG_(L)), MUL_(dL, DIV_(R, L))));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------



// exponential functions
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double funcEXP(op_context *op_params) { return exp(op_params->node_left->item.num); }
node_t *diffEXP(op_context *op_params) { return MUL_(EXP_(L), dL); }

double funcLG(op_context *op_params) { return log(op_params->node_left->item.num); }
node_t *diffLG(op_context *op_params) { return DIV_(dL, L); }

double funcLOG(op_context *op_params) { return log(op_params->node_right->item.num) / log(op_params->node_left->item.num); }
node_t *diffLOG(op_context *op_params) { return DIV_(dR, MUL_(L, LG_(L))); }
//---------------------------------------------------------------------------------------------------------------------------------------------------------



// trigonometric functions
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double funcSIN(op_context *op_params) { return sin(op_params->node_left->item.num); }
node_t *diffSIN(op_context *op_params) { return MUL_(COS_(L), dL); }

double funcCOS(op_context *op_params) { return cos(op_params->node_left->item.num); }
node_t *diffCOS(op_context *op_params) { return MUL_(NUM_(-1), MUL_(SIN_(L), dL)); }

double funcTAN(op_context *op_params) { return tan(op_params->node_left->item.num); }
node_t *diffTAN(op_context *op_params) { return DIV_(dL, MUL_(COS_(L), COS_(L))); }

double funcCOT(op_context *op_params) { return 1 / tan(op_params->node_left->item.num); }
node_t *diffCOT(op_context *op_params) { return MUL_(DIV_(NUM_(-1), MUL_(SIN_(L), SIN_(L))), dL); }

//---------------------------------------------------------------------------------------------------------------------------------------------------------



// hyperbolic functions
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double funcSINH(op_context *op_params) { return (exp(op_params->node_left->item.num) - exp(-op_params->node_left->item.num)) / 2; }
node_t *diffSINH(op_context *op_params) { return MUL_(COSH_(L), dL); }

double funcCOSH(op_context *op_params) { return (exp(op_params->node_left->item.num) + exp(-op_params->node_left->item.num)) / 2; }
node_t *diffCOSH(op_context *op_params) { return MUL_(SINH_(L), dL); }

double funcTANH(op_context *op_params) { return funcSINH(op_params) / funcCOSH(op_params); }
node_t *diffTANH(op_context *op_params) { return DIV_(dL, MUL_(COSH_(L), COSH_(L))); }

double funcCOTH(op_context *op_params) { return funcCOSH(op_params) / funcSINH(op_params); }
node_t *diffCOTH(op_context *op_params) { return MUL_(DIV_(NUM_(-1), MUL_(SINH_(L), SINH_(L))), dL); }

//---------------------------------------------------------------------------------------------------------------------------------------------------------



// inverse trigonometric functions
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double funcARCSIN(op_context *op_params) { return asin(op_params->node_left->item.num); }
node_t *diffARCSIN(op_context *op_params) { return DIV_(dL, POW_(SUB_(NUM_(1), MUL_(L, L)), NUM_(0.5))); }

double funcARCCOS(op_context *op_params) { return acos(op_params->node_left->item.num); }
node_t *diffARCCOS(op_context *op_params) { return MUL_(DIV_(NUM_(-1), POW_(SUB_(NUM_(1), MUL_(L, L)), NUM_(0.5))), dL); }

double funcARCTAN(op_context *op_params) { return atan(op_params->node_left->item.num); }
node_t *diffARCTAN(op_context *op_params) { return DIV_(dL, ADD_(NUM_(1), MUL_(L, L))); }

double funcARCCOT(op_context *op_params) { return acot(op_params->node_left->item.num); }
node_t *diffARCCOT(op_context *op_params) { return MUL_(DIV_(NUM_(-1), ADD_(NUM_(1), MUL_(L, L))), dL); }

//---------------------------------------------------------------------------------------------------------------------------------------------------------


// inverse hyperbolic functions
//---------------------------------------------------------------------------------------------------------------------------------------------------------

double funcARCSINH(op_context *op_params) { return asinh(op_params->node_left->item.num); }
node_t *diffARCSINH(op_context *op_params) { return DIV_(dL, POW_(ADD_(NUM_(1), MUL_(L, L)), NUM_(0.5))); }

double funcARCCOSH(op_context *op_params) { return acosh(op_params->node_left->item.num); }
node_t *diffARCCOSH(op_context *op_params) { return MUL_(DIV_(NUM_(-1), POW_(SUB_(MUL_(L, L), NUM_(1)), NUM_(0.5))), dL); }

double funcARCTANH(op_context *op_params) { return atanh(op_params->node_left->item.num); }
node_t *diffARCTANH(op_context *op_params) { return DIV_(dL, SUB_(NUM_(1), MUL_(L, L))); }

double funcARCCOTH(op_context *op_params) { return acoth(op_params->node_left->item.num); }
node_t *diffARCCOTH(op_context *op_params) { return DIV_(dL, SUB_(NUM_(1), MUL_(L, L))); }

//---------------------------------------------------------------------------------------------------------------------------------------------------------