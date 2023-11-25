#include "differentiator_common.h"

#include <assert.h>

void diff_print_status_message( FILE *stream, DiffStatus status )
{
    fprintf(stream, "%s", diff_status_messages[status]);
}

ExprNodeData diff_get_data( const TreeNode *node_ptr )
{
    assert(node_ptr);

    return *((ExprNodeData *)( tree_get_data_ptr( node_ptr ) ));
}

ExprNodeType diff_get_type( const TreeNode *node_ptr )
{
    assert(node_ptr);

    return diff_get_data(node_ptr).type;
}

double diff_get_const( Expression *expr_ptr, TreeNode *node_ptr )
{
    assert(node_ptr);

    return diff_get_data(node_ptr).cnst;
}

void diff_write_const( Expression *expr_ptr, TreeNode *node_ptr, double cnst )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { CONST, cnst };
    tree_change_data( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_const_at_left( Expression *expr_ptr, TreeNode *node_ptr, double cnst )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { CONST, cnst };
    tree_insert_data_as_left_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_const_at_right( Expression *expr_ptr, TreeNode *node_ptr, double cnst )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { CONST, cnst };
    tree_insert_data_as_right_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}

var_t diff_get_var( Expression *expr_ptr, TreeNode *node_ptr )
{
    assert(expr_ptr);
    assert(node_ptr);

    return diff_get_data(node_ptr).var;
}

void diff_write_var( Expression *expr_ptr, TreeNode *node_ptr, var_t var )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { VAR, var };
    tree_change_data( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_var_at_left( Expression *expr_ptr, TreeNode *node_ptr, var_t var )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { VAR, var };
    tree_insert_data_as_left_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_var_at_right( Expression *expr_ptr, TreeNode *node_ptr, var_t var )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { VAR, var };
    tree_insert_data_as_right_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}

op_unr_t diff_get_op_unr( Expression *expr_ptr, TreeNode *node_ptr )
{
    assert(expr_ptr);
    assert(node_ptr);

    return diff_get_data(node_ptr).op_unr;
}

void diff_write_op_unr( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { OP_UNR, op_unr };
    tree_change_data( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_op_unr_at_left( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { OP_UNR, op_unr };
    tree_insert_data_as_left_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_op_unr_at_right( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { OP_UNR, op_unr };
    tree_insert_data_as_right_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}

op_bin_t diff_get_op_bin( Expression *expr_ptr, TreeNode *node_ptr )
{
    assert(expr_ptr);
    assert(node_ptr);

    return diff_get_data(node_ptr).op_bin;
}

void diff_write_op_bin( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { OP_BIN, op_bin };
    tree_change_data( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_op_bin_at_left( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { OP_BIN, op_bin };
    tree_insert_data_as_left_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}

void diff_insert_op_bin_at_right( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin )
{
    assert(expr_ptr);
    assert(node_ptr);

    ExprNodeData node_data = { OP_BIN, op_bin };
    tree_insert_data_as_right_child( &expr_ptr->expr_tree, node_ptr, &node_data );
}
