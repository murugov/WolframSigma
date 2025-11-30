#ifndef EXPRESS_READER_H
#define EXPRESS_READER_H

#include <stdio.h>

enum ebnfErr_t
{
    EBNF_SUCCESS = 0,
    EBNF_ERROR   = 1
};


ebnfErr_t GetG(double *val);  // ~double
ebnfErr_t GetE(double *val);
ebnfErr_t GetT(double *val);
ebnfErr_t GetP(double *val);
ebnfErr_t GetN(double *val);

#endif