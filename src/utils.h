#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>


const double DBL_PRECISION = 1E-36;


void clear_inp(FILE *stream);

int is_dbl_zero(double x);

int are_dbls_equal(double a, double b);

#endif
