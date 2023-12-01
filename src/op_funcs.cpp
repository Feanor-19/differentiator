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




void diff_op_add( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    diff_insert_op_bin( new_tree, parent_node_ptr, OP_ADD, child );

    TreeNode *node = diff_get_child(new_tree, parent_node_ptr, child);

    diff_node(  src_tree,
                diff_get_child(src_tree, node_to_diff, LEFT),
                new_tree,
                node,
                LEFT,
                diff_by );

    diff_node(  src_tree,
                diff_get_child(src_tree, node_to_diff, RIGHT),
                new_tree,
                node,
                RIGHT,
                diff_by );
}
void diff_op_sub( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);


    diff_insert_op_bin( new_tree, parent_node_ptr, OP_SUB, child );

    TreeNode *node = diff_get_child(new_tree, parent_node_ptr, child);

    diff_node(  src_tree,
                diff_get_child(src_tree, node_to_diff, LEFT),
                new_tree,
                node,
                LEFT,
                diff_by );

    diff_node(  src_tree,
                diff_get_child(src_tree, node_to_diff, RIGHT),
                new_tree,
                node,
                RIGHT,
                diff_by );
}
void diff_op_mul( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);

    diff_insert_op_bin( new_tree, parent_node_ptr, OP_ADD, child );
    TreeNode *node_add = diff_get_child(new_tree, parent_node_ptr, child);

    diff_insert_op_bin( new_tree, node_add, OP_MUL, LEFT );
    TreeNode *mul_left = diff_get_child(new_tree, node_add, LEFT);

    diff_insert_op_bin( new_tree, node_add, OP_MUL, RIGHT );
    TreeNode *mul_right = diff_get_child(new_tree, node_add, RIGHT);

    // left mul
    diff_node(  src_tree,
                diff_get_child(src_tree, node_to_diff, LEFT),
                new_tree,
                mul_left,
                LEFT,
                diff_by );

    tree_copy_subtree_into_right( new_tree, mul_left, diff_get_child(src_tree, node_to_diff, RIGHT) );

    //right mul
    tree_copy_subtree_into_left( new_tree, mul_right, diff_get_child(src_tree, node_to_diff, LEFT) );

    diff_node(  src_tree,
                diff_get_child(src_tree, node_to_diff, RIGHT),
                new_tree,
                mul_right,
                RIGHT,
                diff_by );
}

void diff_op_div( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);


}
void diff_op_pow( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);


}



void diff_op_plus( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);


}
void diff_op_minus( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);


}
void diff_op_sqrt( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by )
{
    assert(src_tree);
    assert(node_to_diff);
    assert(new_tree);


}
