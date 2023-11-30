#include "differentiator_output.h"

#include <assert.h>

static DiffStatus print_expr_tree_node( FILE *stream,
                                        const Expression *expr_ptr,
                                        const TreeNode *node_ptr,
                                        op_prior_t prev_op_prior)
{
    assert(expr_ptr);
    assert(node_ptr);

    op_prior_t prior = 0;
    switch (diff_get_type(node_ptr))
    {
    case CONST:
        fprintf(stream, "%g", diff_get_const(node_ptr));
        break;
    case VAR:
        fprintf(stream, "%s", expr_ptr->vars_names[ diff_get_var(node_ptr) ]);
        break;
    case OP_UNR:
        prior = op_unr_list[diff_get_op_unr(node_ptr)].prior;
        fprintf(stream, " %s(", op_unr_list[diff_get_op_unr(node_ptr)].name);
        print_expr_tree_node(stream, expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(stream, ")");
        break;
    case OP_BIN:
        prior = op_bin_list[diff_get_op_bin(node_ptr)].prior;
        if (prev_op_prior < prior)
            fprintf(stream, "(");

        print_expr_tree_node(stream, expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(stream, " %s ", op_bin_list[diff_get_op_bin(node_ptr)].name);
        print_expr_tree_node(stream, expr_ptr, tree_get_right_child(node_ptr), prior);

        if (prev_op_prior < prior)
            fprintf(stream, ")");
        break;
    case ERROR:
        fprintf(stream, "ERROR_NODE");
        break;
    default:
        assert(0);
    }
    return DIFF_STATUS_OK;
}

DiffStatus diff_print_expr( FILE *stream, const Expression *expr_ptr )
{
    assert(stream);
    assert(expr_ptr);

    DiffStatus err = print_expr_tree_node( stream,
                                           expr_ptr,
                                           tree_get_root( &expr_ptr->expr_tree ),
                                           0 );
    return err;
}
