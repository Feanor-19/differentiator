#include "differentiator_output.h"

#include <assert.h>

static DiffStatus print_expr_tree_node( FILE *stream,
                                        const Expression *expr_ptr,
                                        const TreeNode *node_ptr,
                                        op_prior_t prev_op_prior)
{
    assert(expr_ptr);
    assert(node_ptr);

    switch (diff_get_type(node_ptr))
    {
    case CONST:
        fprintf(stream, "%lf", diff_get_const(node_ptr));
        break;
    case VAR:
        fprintf(stream, "%s", expr_ptr->vars_names[ diff_get_var(node_ptr) ]);
        break;
    // case
    // TODO -
    case ERROR:
        fprintf(stream, "ERROR_NODE");
        break;
    default:
        assert(0);
    }
}

DiffStatus diff_print_expr( FILE *stream, const Expression *expr_ptr )
{
    assert(stream);
    assert(expr_ptr);

    DiffStatus err = print_expr_tree_node( stream, expr_ptr, tree_get_root( &expr_ptr->expr_tree ), 0 );
}
