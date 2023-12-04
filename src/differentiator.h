#ifndef DIFF_H
#define DIFF_H

#include "differentiator_common.h"


double diff_evaluate( Expression *expr_ptr, const double *var_values );

void diff_node( const Tree *src_tree,
                const TreeNode *diff_node,
                Tree *new_tree,
                TreeNode *parent_node_ptr,
                Child child,
                var_t diff_by );

Expression diff_diff( const Expression *expr_ptr, var_t diff_by );

//! @brief Folds costants in the subtree, which starts with the given node.
//! Returns 0, if no changes were done, and 1 otherwise.
int diff_fold_constants( Expression *expr_ptr, TreeNode *node_ptr );

//! @brief Folds neutrals in the subtree, which starts with the given node.
//! Returns 0, if no changes were done, and 1 otherwise.
//! @param [in] expr_ptr Pointer to the expression.
//! @param [in] node_ptr Node the specified subtree starts with.
int diff_fold_neutrals( Expression *expr_ptr, TreeNode *node_ptr );

void diff_simplify( Expression *expr_ptr );

//! @brief Returns 1 if subtree, which starts with 'subtree' node,
//! CONTAINS variable 'var', and 0 otherwise.
int is_subtree_var( const Expression *expr_ptr, const TreeNode *subtree, var_t var );

void diff_expr_dtor( Expression *expr_ptr );

#endif /* DIFF_H */
