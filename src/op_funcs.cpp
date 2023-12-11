#include "op_funcs.h"

#include "differentiator.h"

#include <assert.h>

double op_bin_add(double a, double b)
{
    return a + b;
}

double op_bin_sub(double a, double b)
{
    return a - b;
}

double op_bin_mul(double a, double b)
{
    return a * b;
}

double op_bin_div(double a, double b)
{
    return a / b;
}

double op_bin_pow(double a, double b)
{
    return pow(a, b);
}



double op_unr_plus(double a)
{
    return a;
}

double op_unr_minus(double a)
{
    return -a;
}

double op_unr_sqrt(double a)
{
    return sqrt(a);
}

double op_unr_exp(double a)
{
    return exp(a);
}

double op_unr_ln(double a)
{
    return log(a);
}

double op_unr_sin(double a)
{
    return sin(a);
}

double op_unr_cos(double a)
{
    return cos(a);
}


#define _INSERT_OP_BIN_AND_GET( node_where, op, as_which_child )      \
    ( diff_insert_op_bin( new_tree, node_where, op, as_which_child ),   \
     diff_get_child(new_tree, node_where, as_which_child))

#define _INSERT_OP_UNR_AND_GET( node_where, op, as_which_child )      \
    ( diff_insert_op_unr( new_tree, node_where, op, as_which_child ),   \
     diff_get_child(new_tree, node_where, as_which_child))

#define _L diff_get_child(src_tree, node_to_diff, LEFT)

#define _R diff_get_child(src_tree, node_to_diff, RIGHT)

#define _INSERT_DIFFED( node_to_diff, node_where, as_which_child ) \
    diff_node( src_tree, node_to_diff, new_tree, node_where, as_which_child, diff_by );

#define _INSERT_COPY( node_to_copy, node_where, as_which_child )            \
    if ( as_which_child == LEFT )                                           \
        tree_copy_subtree_into_left( new_tree, node_where, node_to_copy );  \
    else if ( as_which_child == RIGHT )                                     \
        tree_copy_subtree_into_right( new_tree, node_where, node_to_copy ); \
    else assert(0);


void diff_op_add( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    TreeNode *node = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_ADD, child );

    _INSERT_DIFFED( _L, node, LEFT );

    _INSERT_DIFFED( _R, node, RIGHT );
}
void diff_op_sub( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    TreeNode *node = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_SUB, child );

    _INSERT_DIFFED( _L, node, LEFT );

    _INSERT_DIFFED( _R, node, RIGHT );
}
void diff_op_mul( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    TreeNode *node_add = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_ADD, child );

    TreeNode *mul_left = _INSERT_OP_BIN_AND_GET( node_add, OP_MUL, LEFT );

    TreeNode *mul_right = _INSERT_OP_BIN_AND_GET( node_add, OP_MUL, RIGHT );

    _INSERT_DIFFED( _L, mul_left, LEFT );

    _INSERT_COPY( _R, mul_left, RIGHT );

    _INSERT_COPY( _L, mul_right, LEFT );

    _INSERT_DIFFED( _R, mul_right, RIGHT );
}

void diff_op_div( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    TreeNode *node_div = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_DIV, child );

    TreeNode *node_sub = _INSERT_OP_BIN_AND_GET( node_div, OP_SUB, LEFT );

    TreeNode *node_mul_left = _INSERT_OP_BIN_AND_GET( node_sub, OP_MUL, LEFT );

    TreeNode *node_mul_right = _INSERT_OP_BIN_AND_GET( node_sub, OP_MUL, RIGHT );

    TreeNode *node_pow = _INSERT_OP_BIN_AND_GET( node_div, OP_POW, RIGHT );

    _INSERT_DIFFED( _L, node_mul_left, LEFT );

    _INSERT_COPY( _R, node_mul_left, RIGHT );

    _INSERT_COPY( _L, node_mul_right, LEFT );

    _INSERT_DIFFED( _R, node_mul_right, RIGHT );

    _INSERT_COPY( _R, node_pow, LEFT );

    diff_insert_const( new_tree, node_pow, 2, RIGHT );

}

void diff_op_pow( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    if ( !is_subtree_var( _L, diff_by ) && !is_subtree_var( _R, diff_by ) )
    {
        diff_insert_const( new_tree, parent_node_ptr, 0, child );
    }
    else if ( is_subtree_var( _L, diff_by ) && !is_subtree_var( _R, diff_by ) )
    {
        TreeNode *node_mul1 = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_MUL, child );
        _INSERT_DIFFED( _L, node_mul1, RIGHT );
        TreeNode *node_mul2 = _INSERT_OP_BIN_AND_GET( node_mul1, OP_MUL, LEFT );
        _INSERT_COPY( _R, node_mul2, LEFT );
        TreeNode *node_pow = _INSERT_OP_BIN_AND_GET( node_mul2, OP_POW, RIGHT );
        _INSERT_COPY( _L, node_pow, LEFT );
        TreeNode *node_sub = _INSERT_OP_BIN_AND_GET( node_pow, OP_SUB, RIGHT );
        _INSERT_COPY( _R, node_sub, LEFT );
        diff_insert_const( new_tree, node_sub, 1, RIGHT );
    }
    else if ( !is_subtree_var( _L, diff_by ) && is_subtree_var( _R, diff_by ) )
    {
        TreeNode *node_mul1 = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_MUL, child );

        _INSERT_COPY( node_to_diff, node_mul1, LEFT );

        TreeNode *node_mul2 = _INSERT_OP_BIN_AND_GET( node_mul1, OP_MUL, RIGHT );
        TreeNode *node_ln = _INSERT_OP_UNR_AND_GET( node_mul2, OP_LN, LEFT );
        _INSERT_COPY( _L, node_ln, LEFT );
        _INSERT_DIFFED( _R, node_mul2, RIGHT );
    }
    else if ( is_subtree_var( _L, diff_by ) && is_subtree_var( _R, diff_by ) )
    {
        TreeNode *node_mul1 = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_MUL, child );

        _INSERT_COPY( node_to_diff, node_mul1, LEFT );

        TreeNode *node_add = _INSERT_OP_BIN_AND_GET( node_mul1, OP_ADD, RIGHT );

        TreeNode *node_mul2 = _INSERT_OP_BIN_AND_GET( node_add, OP_MUL, LEFT );
        TreeNode *node_div = _INSERT_OP_BIN_AND_GET( node_mul2, OP_DIV, LEFT );
        _INSERT_DIFFED( _L, node_div, LEFT );
        _INSERT_COPY( _L, node_div, RIGHT );
        _INSERT_COPY( _R, node_mul2, RIGHT );

        TreeNode *node_mul3 = _INSERT_OP_BIN_AND_GET( node_add, OP_MUL, RIGHT );
        TreeNode *node_ln = _INSERT_OP_UNR_AND_GET( node_mul3, OP_LN, LEFT );
        _INSERT_COPY( _L, node_ln, LEFT );
        _INSERT_DIFFED( _R, node_mul3, RIGHT );
    }
}



void diff_op_plus( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    TreeNode *node_plus = _INSERT_OP_UNR_AND_GET(parent_node_ptr, OP_PLUS, child);

    _INSERT_DIFFED( _L, node_plus, LEFT );
}
void diff_op_minus( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    TreeNode *node_minus = _INSERT_OP_UNR_AND_GET(parent_node_ptr, OP_MINUS, child);

    _INSERT_DIFFED( _L, node_minus, LEFT );
}
void diff_op_sqrt( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    TreeNode *node_div = _INSERT_OP_BIN_AND_GET(parent_node_ptr, OP_DIV, child);

    _INSERT_DIFFED( _L, node_div, LEFT );

    TreeNode *node_mul = _INSERT_OP_BIN_AND_GET( node_div, OP_MUL, RIGHT );

    diff_insert_const(new_tree, node_mul, 2, LEFT);

    TreeNode *node_sqrt = _INSERT_OP_UNR_AND_GET(node_mul, OP_SQRT, RIGHT);

    _INSERT_COPY(_L, node_sqrt, LEFT);
}

void diff_op_exp( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    TreeNode *node_mul = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_MUL, child );
    TreeNode *node_exp = _INSERT_OP_UNR_AND_GET( node_mul, OP_EXP, LEFT );
    _INSERT_COPY( _L, node_exp, LEFT );
    _INSERT_DIFFED( _L, node_mul, RIGHT );
}

void diff_op_ln( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    TreeNode *node_div = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_DIV, child );
    _INSERT_DIFFED( _L, node_div, LEFT );
    _INSERT_COPY( _L, node_div, RIGHT );
}

void diff_op_sin( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    TreeNode *node_mul = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_MUL, child );
    TreeNode *node_cos = _INSERT_OP_UNR_AND_GET( node_mul, OP_COS, LEFT );
    _INSERT_COPY( _L, node_cos, LEFT );
    _INSERT_DIFFED( _L, node_mul, RIGHT );
}

void diff_op_cos( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    TreeNode *node_mul = _INSERT_OP_BIN_AND_GET( parent_node_ptr, OP_MUL, child );
    TreeNode *node_minus = _INSERT_OP_UNR_AND_GET( node_mul, OP_MINUS, LEFT );
    TreeNode *node_sin = _INSERT_OP_UNR_AND_GET( node_minus, OP_SIN, LEFT );
    _INSERT_COPY( _L, node_sin, LEFT );
    _INSERT_DIFFED( _L, node_mul, RIGHT );
}
