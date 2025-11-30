#include "ExpressReader.h"


const char* express = "6*/12$";

int main()
{
    double res = 0.0;
    printf("verd = %d\n", GetG(&res));
    printf("res = %g\n", res);
}


ebnfErr_t GetG(double *val)
{
    ebnfErr_t syntax_verd = GetE(val);
    if (*express != '$')
        return EBNF_ERROR;
        
    express++;
    return syntax_verd;
}

ebnfErr_t GetE(double *val)
{
    double val_1 = 0.0;
    GetT(&val_1);

    while (*express == '+' || *express == '-')
    {
        int op = *express;
        express++;
        double val_2 = 0.0;
        GetT(&val_2);
        if (op == '+')
            val_1 += val_2;
        else
            val_1 -= val_2;
    }

    *val = val_1;
    return EBNF_SUCCESS;
}

ebnfErr_t GetT(double *val)
{
    double val_1 = 0.0;
    GetP(&val_1);

    while (*express == '*' || *express == '/')
    {
        int op = *express;
        express++;
        double val_2 = 0.0;
        GetP(&val_2);
        if (op == '*')
            val_1 *= val_2;
        else
            val_1 /= val_2;
    }

    *val = val_1;
    return EBNF_SUCCESS;
}

ebnfErr_t GetP(double *val)
{
    if (*express == '(')
    {
        express++;
        GetE(val);

        if (*express == ')')
        {
            express++;
            return EBNF_SUCCESS;
        }

        return EBNF_ERROR;
    }

    return GetN(val);
}

ebnfErr_t GetN(double *val)
{
    int num_digits = 0;
    if (sscanf(express, "%lg%n", val, &num_digits) != 1)
        return EBNF_ERROR;

    express += num_digits;
    return EBNF_SUCCESS;
}