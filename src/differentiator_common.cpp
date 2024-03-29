#include "differentiator_common.h"

#include "differentiator.h"

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

TreeNode* diff_get_child( const Tree* tree_ptr, const TreeNode* node_ptr, Child child )
{
    assert(tree_ptr);

    switch (child)
    {
    case ROOT:
        return tree_get_root(tree_ptr);
        break;
    case LEFT:
        return tree_get_left_child(node_ptr);
        break;
    case RIGHT:
        return tree_get_right_child(node_ptr);
        break;
    default:
        assert(0);
        break;
    }
    assert(0);
    return NULL;
}

double diff_get_const( const TreeNode *node_ptr )
{
    assert(node_ptr);

    return diff_get_data(node_ptr).cnst;
}

void diff_write_const( Tree *expr_tree, TreeNode *node_ptr, double cnst )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = CONST;
    node_data.cnst = cnst;
    tree_change_data( expr_tree, node_ptr, &node_data );
}

void diff_insert_const_as_root( Tree *expr_tree, double cnst )
{
    assert(expr_tree);

    ExprNodeData node_data = {};
    node_data.type = CONST;
    node_data.cnst = cnst;
    tree_insert_root( expr_tree, &node_data );
}

void diff_insert_const_at_left( Tree *expr_tree, TreeNode *node_ptr, double cnst )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = CONST;
    node_data.cnst = cnst;
    tree_insert_data_as_left_child(expr_tree, node_ptr, &node_data );
}

void diff_insert_const_at_right( Tree *expr_tree, TreeNode *node_ptr, double cnst )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = CONST;
    node_data.cnst = cnst;
    tree_insert_data_as_right_child(expr_tree, node_ptr, &node_data );
}

void diff_insert_const( Tree *expr_tree, TreeNode *node_ptr, double cnst, Child child )
{
    assert(expr_tree);

    switch (child)
    {
    case ROOT:
        diff_insert_const_as_root(expr_tree, cnst);
        break;
    case LEFT:
        diff_insert_const_at_left(expr_tree, node_ptr, cnst);
        break;
    case RIGHT:
        diff_insert_const_at_right(expr_tree, node_ptr, cnst);
        break;
    default:
        assert(0);
        break;
    }
}

TreeNode *diff_new_const_node( Tree *expr_tree, double cnst )
{
    assert(expr_tree);

    ExprNodeData data = {};
    data.type = CONST;
    data.cnst = cnst;
    return op_new_TreeNode( expr_tree, &data );
}

var_t diff_get_var( const TreeNode *node_ptr )
{
    assert(node_ptr);

    return diff_get_data(node_ptr).var;
}

void diff_write_var( Tree *expr_tree, TreeNode *node_ptr, var_t var )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = VAR;
    node_data.var = var;
    tree_change_data(expr_tree, node_ptr, &node_data );
}

void diff_insert_var_as_root( Tree *expr_tree, var_t var )
{
    assert(expr_tree);

    ExprNodeData node_data = {};
    node_data.type = VAR;
    node_data.var = var;
    tree_insert_root(expr_tree, &node_data );
}

void diff_insert_var_at_left( Tree *expr_tree, TreeNode *node_ptr, var_t var )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = VAR;
    node_data.var = var;
    tree_insert_data_as_left_child(expr_tree, node_ptr, &node_data );
}

void diff_insert_var_at_right( Tree *expr_tree, TreeNode *node_ptr, var_t var )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = VAR;
    node_data.var = var;
    tree_insert_data_as_right_child(expr_tree, node_ptr, &node_data );
}

void diff_insert_var( Tree *expr_tree, TreeNode *node_ptr, var_t var, Child child )
{
    assert(expr_tree);

    switch (child)
    {
    case ROOT:
        diff_insert_var_as_root(expr_tree, var);
        break;
    case LEFT:
        diff_insert_var_at_left(expr_tree, node_ptr, var);
        break;
    case RIGHT:
        diff_insert_var_at_right(expr_tree, node_ptr, var);
        break;
    default:
        assert(0);
        break;
    }
}

TreeNode *diff_new_var_node( Tree *expr_tree, var_t var )
{
    assert(expr_tree);

    ExprNodeData data = {};
    data.type = VAR;
    data.var = var;
    return op_new_TreeNode( expr_tree, &data );
}

op_unr_t diff_get_op_unr( const TreeNode *node_ptr )
{
    assert(node_ptr);

    return diff_get_data(node_ptr).op_unr;
}

void diff_write_op_unr( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = OP_UNR;
    node_data.op_unr = op_unr;
    tree_change_data(expr_tree, node_ptr, &node_data );
}

void diff_insert_op_unr_as_root( Tree *expr_tree, op_unr_t op_unr )
{
    assert(expr_tree);

    ExprNodeData node_data = {};
    node_data.type = OP_UNR;
    node_data.op_unr = op_unr;
    tree_insert_root(expr_tree, &node_data );
}

void diff_insert_op_unr_at_left( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = OP_UNR;
    node_data.op_unr = op_unr;
    tree_insert_data_as_left_child(expr_tree, node_ptr, &node_data );
}

void diff_insert_op_unr_at_right( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = OP_UNR;
    node_data.op_unr = op_unr;
    tree_insert_data_as_right_child( expr_tree, node_ptr, &node_data );
}

void diff_insert_op_unr( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr, Child child )
{
    assert(expr_tree);

    switch (child)
    {
    case ROOT:
        diff_insert_op_unr_as_root(expr_tree, op_unr);
        break;
    case LEFT:
        diff_insert_op_unr_at_left(expr_tree, node_ptr, op_unr);
        break;
    case RIGHT:
        diff_insert_op_unr_at_right(expr_tree, node_ptr, op_unr);
        break;
    default:
        assert(0);
        break;
    }
}

TreeNode *diff_new_op_unr_node( Tree *expr_tree, op_unr_t op_unr )
{
    assert(expr_tree);

    ExprNodeData data = {};
    data.type = OP_UNR;
    data.op_unr = op_unr;
    return op_new_TreeNode( expr_tree, &data );
}

op_bin_t diff_get_op_bin( const TreeNode *node_ptr )
{
    assert(node_ptr);

    return diff_get_data(node_ptr).op_bin;
}

void diff_write_op_bin( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = OP_BIN;
    node_data.op_bin = op_bin;
    tree_change_data(expr_tree, node_ptr, &node_data );
}

void diff_insert_op_bin_as_root( Tree *expr_tree, op_bin_t op_bin )
{
    assert(expr_tree);

    ExprNodeData node_data = {};
    node_data.type = OP_BIN;
    node_data.op_bin = op_bin;
    tree_insert_root(expr_tree, &node_data );
}

void diff_insert_op_bin_at_left( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = OP_BIN;
    node_data.op_bin = op_bin;
    tree_insert_data_as_left_child( expr_tree, node_ptr, &node_data );
}

void diff_insert_op_bin_at_right( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin )
{
    assert(expr_tree);
    assert(node_ptr);

    ExprNodeData node_data = {};
    node_data.type = OP_BIN;
    node_data.op_bin = op_bin;
    tree_insert_data_as_right_child( expr_tree, node_ptr, &node_data );
}

void diff_insert_op_bin( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin, Child child )
{
    assert(expr_tree);

    switch (child)
    {
    case ROOT:
        diff_insert_op_bin_as_root(expr_tree, op_bin);
        break;
    case LEFT:
        diff_insert_op_bin_at_left(expr_tree, node_ptr, op_bin);
        break;
    case RIGHT:
        diff_insert_op_bin_at_right(expr_tree, node_ptr, op_bin);
        break;
    default:
        assert(0);
        break;
    }
}

TreeNode *diff_new_op_bin_node( Tree *expr_tree, op_bin_t op_bin )
{
    assert(expr_tree);

    ExprNodeData data = {};
    data.type = OP_BIN;
    data.op_bin = op_bin;
    return op_new_TreeNode( expr_tree, &data );
}

void expr_node_data_print( FILE* stream, void *data_ptr )
{
    assert(stream);
    assert(data_ptr);

    ExprNodeData *ptr = (ExprNodeData*) data_ptr;
    fprintf(stream, "[");
    switch(ptr->type)
    {
        case ERROR:
            fprintf(stream, "ERROR");
            break;
        case CONST:
            fprintf(stream, "type: CONST; value: %lf", ptr->cnst);
            break;
        case VAR:
            fprintf(stream, "type: VAR; var_id: %d", ptr->var);
            break;
        case OP_UNR:
            fprintf(stream, "type: OP_UNR; op_unr_id: %d, %s", ptr->op_unr, op_unr_list[ptr->op_unr].name);
            break;
        case OP_BIN:
            fprintf(stream, "type: OP_BIN; op_bin_id: %d, %s", ptr->op_bin, op_bin_list[ptr->op_bin].name);
            break;
        default:
            assert(0);
            break;
    }
    fprintf(stream, "]");
}
