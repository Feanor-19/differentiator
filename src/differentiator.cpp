#include "differentiator.h"

#include <assert.h>

#include "utils.h"

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
                const TreeNode *node_to_diff,
                Tree *new_tree,
                TreeNode *parent_node_ptr,
                Child child,
                var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    switch (diff_get_type(node_to_diff))
    {
    case CONST:
        diff_insert_const(new_tree, parent_node_ptr, 0, child);
        break;
    case VAR:
        if ( diff_get_var(node_to_diff) == diff_by )
            diff_insert_const(new_tree, parent_node_ptr, 1, child);
        else
            diff_insert_const(new_tree, parent_node_ptr, 0, child);
        break;
    case OP_UNR:
        op_unr_list[ diff_get_op_unr(node_to_diff) ].diff_op(src_tree, node_to_diff,
                                                            new_tree, parent_node_ptr,
                                                            child, diff_by);
        break;
    case OP_BIN:
        op_bin_list[ diff_get_op_bin(node_to_diff) ].diff_op(src_tree, node_to_diff,
                                                            new_tree, parent_node_ptr,
                                                            child, diff_by);
        break;
    case ERROR:
    default:
        assert(0);
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

// TODO - вопрос про оформление свитчей
int diff_fold_constants( Expression *expr_ptr, TreeNode *node_ptr )
{
    assert(expr_ptr);
    assert(node_ptr);

    TreeNode *node_left = NULL;
    TreeNode *node_right = NULL;
    int changes = 0;
    switch (diff_get_type(node_ptr))
    {
    case OP_UNR:
        node_left = tree_get_left_child(node_ptr);

        if ( diff_get_type(node_left) == OP_UNR || diff_get_type(node_left) == OP_BIN )
            changes |= diff_fold_constants(expr_ptr, node_left);

        if ( diff_get_type(node_left) == CONST )
        {
            double a = diff_get_const(node_left);
            double res = op_unr_list[ diff_get_op_unr( node_ptr ) ].op_f(a);
            diff_write_const(&expr_ptr->expr_tree, node_ptr, res);
            tree_delete_left_child(&expr_ptr->expr_tree, node_ptr);
            changes = 1;
        }
        break;
    case OP_BIN:
        node_left = tree_get_left_child(node_ptr);
        node_right = tree_get_right_child(node_ptr);

        if ( diff_get_type(node_left) == OP_UNR || diff_get_type(node_left) == OP_BIN )
            changes |= diff_fold_constants(expr_ptr, node_left);

        if ( diff_get_type(node_right) == OP_UNR || diff_get_type(node_right) == OP_BIN )
            changes |= diff_fold_constants(expr_ptr, node_right);

        if ( diff_get_type(node_left) == CONST && diff_get_type(node_right) == CONST )
        {
            double a = diff_get_const(node_left);
            double b = diff_get_const(node_right);
            double res = op_bin_list[ diff_get_op_bin( node_ptr ) ].op_f(a, b);
            diff_write_const(&expr_ptr->expr_tree, node_ptr, res);
            tree_delete_left_child(&expr_ptr->expr_tree, node_ptr);
            tree_delete_right_child(&expr_ptr->expr_tree, node_ptr);
            changes = 1;
        }
        break;
    case CONST:
    case VAR:
        return 0;
        break;
    case ERROR:
    default:
        assert(0);
        break;
    }

    return changes;
}

inline int fold_neutral_add_sub_zero( Expression *expr_ptr, TreeNode *node_ptr)
{
    TreeNode* node_left = tree_get_left_child(node_ptr);
    TreeNode* node_right = tree_get_right_child(node_ptr);

    if ( diff_get_op_bin( node_ptr ) == OP_ADD || diff_get_op_bin( node_ptr ) == OP_SUB )
    {
        if ( diff_get_type(node_left) == CONST && is_dbl_zero( diff_get_const( node_left ) ) )
        {
            if (!node_ptr->parent)
                tree_migrate_into_root( &expr_ptr->expr_tree, node_right );
            else if ( node_ptr->parent->left == node_ptr )
                tree_migrate_into_left( &expr_ptr->expr_tree, node_ptr->parent, node_right );
            else if ( node_ptr->parent->right == node_ptr )
                tree_migrate_into_right( &expr_ptr->expr_tree, node_ptr->parent, node_right );
            else
                assert(0);
            return 1;
        }
        else if ( diff_get_type(node_right) == CONST && is_dbl_zero( diff_get_const( node_right ) ) )
        {
            if (!node_ptr->parent)
                tree_migrate_into_root( &expr_ptr->expr_tree, node_left );
            else if ( node_ptr->parent->left == node_ptr )
                tree_migrate_into_left( &expr_ptr->expr_tree, node_ptr->parent, node_left );
            else if ( node_ptr->parent->right == node_ptr )
                tree_migrate_into_right( &expr_ptr->expr_tree, node_ptr->parent, node_left );
            else
                assert(0);
            return 1;
        }
    }
    return 0;
}

inline int fold_neutrals_mul_one( Expression *expr_ptr, TreeNode *node_ptr )
{
    TreeNode* node_left = tree_get_left_child(node_ptr);
    TreeNode* node_right = tree_get_right_child(node_ptr);

    if ( diff_get_op_bin( node_ptr ) == OP_MUL )
    {
        if ( diff_get_type(node_left) == CONST && are_dbls_equal( diff_get_const( node_left ), 1 ) )
        {
            if (!node_ptr->parent)
                tree_migrate_into_root( &expr_ptr->expr_tree, node_right );
            else if ( node_ptr->parent->left == node_ptr )
                tree_migrate_into_left( &expr_ptr->expr_tree, node_ptr->parent, node_right );
            else if ( node_ptr->parent->right == node_ptr )
                tree_migrate_into_right( &expr_ptr->expr_tree, node_ptr->parent, node_right );
            else
                assert(0);
            return 1;
        }
        else if ( diff_get_type(node_right) == CONST && are_dbls_equal( diff_get_const( node_right ), 1 ) )
        {
            if (!node_ptr->parent)
                tree_migrate_into_root( &expr_ptr->expr_tree, node_left );
            else if ( node_ptr->parent->left == node_ptr )
                tree_migrate_into_left( &expr_ptr->expr_tree, node_ptr->parent, node_left );
            else if ( node_ptr->parent->right == node_ptr )
                tree_migrate_into_right( &expr_ptr->expr_tree, node_ptr->parent, node_left );
            else
                assert(0);
            return 1;
        }
    }
    return 0;
}

inline int fold_neutrals_mul_zero( Expression *expr_ptr, TreeNode *node_ptr )
{
    TreeNode* node_left = tree_get_left_child(node_ptr);
    TreeNode* node_right = tree_get_right_child(node_ptr);

    if ( diff_get_op_bin( node_ptr ) == OP_MUL )
    {
        if ( diff_get_type(node_left) == CONST && is_dbl_zero( diff_get_const( node_left ) ) )
        {
            tree_delete_left_child( &expr_ptr->expr_tree, node_ptr );
            tree_delete_subtree( &expr_ptr->expr_tree, tree_get_right_child(node_ptr) );
            diff_write_const( &expr_ptr->expr_tree, node_ptr, 0 );
            return 1;
        }
        else if ( diff_get_type(node_right) == CONST && is_dbl_zero( diff_get_const( node_right ) ) )
        {
            tree_delete_right_child( &expr_ptr->expr_tree, node_ptr );
            tree_delete_subtree( &expr_ptr->expr_tree, tree_get_right_child(node_ptr) );
            diff_write_const( &expr_ptr->expr_tree, node_ptr, 0 );
            return 1;
        }
    }
    return 0;
}

int diff_fold_neutrals( Expression *expr_ptr, TreeNode *node_ptr )
{
    assert(expr_ptr);
    assert(node_ptr);

    TreeNode *node_left = NULL;
    TreeNode *node_right = NULL;
    int changes = 0;
    int curr_change = 0;
    switch (diff_get_type(node_ptr))
    {
    case OP_UNR:
        node_left = tree_get_left_child(node_ptr);

        // TODO -
        break;
    case OP_BIN:
        node_left = tree_get_left_child(node_ptr);
        node_right = tree_get_right_child(node_ptr);

        if ( diff_get_type(node_left) == OP_UNR || diff_get_type(node_left) == OP_BIN )
            changes |= diff_fold_neutrals(expr_ptr, node_left);

        if ( diff_get_type(node_right) == OP_UNR || diff_get_type(node_right) == OP_BIN )
            changes |= diff_fold_neutrals(expr_ptr, node_right);

        curr_change = fold_neutral_add_sub_zero( expr_ptr, node_ptr );

        if (!curr_change)
            curr_change = fold_neutrals_mul_one( expr_ptr, node_ptr);

        if (!curr_change)
            curr_change = fold_neutrals_mul_zero( expr_ptr, node_ptr );

        changes |= curr_change;
        break;
    case CONST:
    case VAR:
        return 0;
        break;
    case ERROR:
    default:
        assert(0);
        break;
    }

    return changes;
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
