#ifndef OP_FUNCS_H
#define OP_FUNCS_H

#include <math.h>

#include "diff_inc_and_defs.h"

double op_bin_add(double a, double b);
double op_bin_sub(double a, double b);
double op_bin_mul(double a, double b);
double op_bin_div(double a, double b);
double op_bin_pow(double a, double b);

double op_unr_plus(double a);
double op_unr_minus(double a);
double op_unr_sqrt(double a);


void diff_op_add( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );
void diff_op_sub( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );
void diff_op_mul( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );
void diff_op_div( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );
void diff_op_pow( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );


void diff_op_plus( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );
void diff_op_minus( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );
void diff_op_sqrt( const Tree *src_tree, const TreeNode *node_to_diff,
                  Tree *new_tree, TreeNode *parent_node_ptr,
                  Child child, var_t diff_by );


#endif /* OP_FUNCS_H */
