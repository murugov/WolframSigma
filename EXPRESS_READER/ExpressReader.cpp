#include "ExpressReader.h"


const char* express = "0.5/12$";

int main()
{
    double res = GetG();
    printf("res = %g\n", res);
}


double GetG()
{
    double val = GetE();
    if (*express != '$')
        return 0;
        
    express++;
    return val;
}

double GetE()
{
    double val_1 = GetT();
    while (*express == '+' || *express == '-')
    {
        int op = *express;
        express++;
        double val_2 = GetT();
        if (op == '+')
            val_1 += val_2;
        else
            val_1 -= val_2;
    }

    return val_1;
}

double GetT()
{
    double val_1 = GetP();
    while (*express == '*' || *express == '/')
    {
        int op = *express;
        express++;
        double val_2 = GetP();
        if (op == '*')
            val_1 *= val_2;
        else
            val_1 /= val_2;
    }

    return val_1;
}

double GetP()
{
    if (*express == '(')
    {
        express++;
        double val = GetE();
        //if != ')'
        express++;
        return val;
    }
    return GetN();
}

double GetN()
{
    double val = 0;
    int num_digits = 0;
    if (sscanf(express, "%lg%n", &val, &num_digits) != 1)
    {
        printf("SyntaxError\n");
        return 0.0;
    }

    express += num_digits;
    return val;
}