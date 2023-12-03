#include "utils.h"

#include <ctype.h>
#include <math.h>

void clear_inp(FILE *stream)
{
    while ( getc(stream) != '\n' )
        ;
    return;
}

int is_dbl_zero(double x)
{
    return abs(x) < DBL_PRECISION;
}

int are_dbls_equal(double a, double b)
{
    return is_dbl_zero(a - b);
}
