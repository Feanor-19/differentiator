#ifndef DIFF_H
#define DIFF_H

#include "differentiator_common.h"


enum Child
{
    ROOT,
    LEFT,
    RIGHT,
};


double diff_evaluate( Expression *expr_ptr, const double *var_values );

void diff_node( const Tree *src_tree,
                const TreeNode *diff_node,
                Tree *new_tree,
                TreeNode *parent_node_ptr,
                Child child,
                var_t diff_by );

Expression diff_diff( const Expression *expr_ptr, var_t diff_by );

void diff_expr_dtor( Expression *expr_ptr );

#endif /* DIFF_H */
