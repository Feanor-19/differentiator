#ifndef DIFF_H
#define DIFF_H

#include "differentiator_common.h"


double diff_evaluate( Expression *expr_ptr, const double *var_values );

void diff_expr_dtor( Expression *expr_ptr );

#endif /* DIFF_H */
