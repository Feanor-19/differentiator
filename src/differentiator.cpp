#include "differentiator.h"

#include <assert.h>

inline double eval_node( Expression *expr_ptr, const double *var_values, TreeNode *node_ptr )
{
    assert(expr_ptr);
    assert(var_values);
    assert(node_ptr);

    double a = NAN, b = NAN;
    switch (diff_get_type(node_ptr))
    {
    case CONST:
        return diff_get_const(node_ptr);
        break;
    case VAR:
        return var_values[ diff_get_var(node_ptr) ];
        break;
    case OP_UNR:
        a = eval_node( expr_ptr, var_values, tree_get_left_child(node_ptr) );
        return op_unr_list[ diff_get_op_unr(node_ptr) ].op_f(a);
        break;
    case OP_BIN:
        a = eval_node( expr_ptr, var_values, tree_get_left_child(node_ptr) );
        b = eval_node( expr_ptr, var_values, tree_get_right_child(node_ptr) );
        return op_bin_list[ diff_get_op_bin(node_ptr) ].op_f(a, b);
        break;
    case ERROR:
        // TODO -
        break;
    default:
        assert(0);
        break;
    }
    assert(0);
    return NAN;
}

double diff_evaluate( Expression *expr_ptr, const double *var_values )
{
    assert(expr_ptr);
    assert(var_values);

    return eval_node( expr_ptr, var_values, tree_get_root( &expr_ptr->expr_tree ) );
}

void diff_node( const Tree *src_tree,
                const TreeNode *diff_node,
                Tree *new_tree,
                TreeNode *parent_node_ptr,
                Child child,
                var_t diff_by )
{
    assert(src_tree);
    assert(diff_node);
    assert(new_tree);
    assert(parent_node_ptr);

    switch (diff_get_type(diff_node))
    {
    case CONST:

        break;

    default:
        break;
    }
}

Expression diff_diff( const Expression *expr_ptr, var_t diff_by )
{
    assert(expr_ptr);

    Tree new_tree = {};
    tree_ctor(&new_tree, sizeof(ExprNodeData), NULL, expr_node_data_print);

    // упрощение дерева

    diff_node(  &expr_ptr->expr_tree,
                tree_get_root(&expr_ptr->expr_tree),
                &new_tree,
                NULL,
                ROOT,
                diff_by);

    // упрощение дерева

    Expression new_expr = {};
    new_expr.expr_tree = new_tree;
    new_expr.vars_names_len = expr_ptr->vars_names_len;
    new_expr.vars_names = (char**) calloc( expr_ptr->vars_names_len, sizeof(char*) );
    for (size_t ind = 0; ind < expr_ptr->vars_names_len; ind++)
    {
        new_expr.vars_names[ind] = strdup( expr_ptr->vars_names[ind] );
    }

    return new_expr;
}

void diff_expr_dtor( Expression *expr_ptr )
{
    assert(expr_ptr);

    tree_dtor(&expr_ptr->expr_tree);

    for (size_t ind = 0; ind < expr_ptr->vars_names_len; ind++)
    {
        assert(expr_ptr->vars_names[ind]);
        free(expr_ptr->vars_names[ind]);
    }
    free(expr_ptr->vars_names);
}
