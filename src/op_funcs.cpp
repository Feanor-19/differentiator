#include "op_funcs.h"

//#include "differentiator.h"

#include <assert.h>

double op_bin_add(double a, double b)
{
    return a + b;
}

double op_bin_sub(double a, double b)
{
    return a - b;
}

double op_bin_mul(double a, double b)
{
    return a * b;
}

double op_bin_div(double a, double b)
{
    return a / b;
}

double op_bin_pow(double a, double b)
{
    return pow(a, b);
}



double op_unr_plus(double a)
{
    return a;
}

double op_unr_minus(double a)
{
    return -a;
}

double op_unr_sqrt(double a)
{
    return sqrt(a);
}
