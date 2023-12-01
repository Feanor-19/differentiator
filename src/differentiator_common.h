#ifndef DIFF_COMMON_H
#define DIFF_COMMON_H

#include "diff_inc_and_defs.h"

#include "op_funcs.h"

#define DEF_DIFF_STATUS(name, message) message,
const char * const diff_status_messages[] =
{
    #include "diff_statuses.h"
    "FICTIONAL MESSAGE!"
};
#undef DEF_DIFF_STATUS


/*
    ПРИМЕЧАНИЕ ОБ ИМЕНАХ ОПЕРАТОРОВ:
    Имя оператора может:
    - ИЛИ состоять из одного символа, не являющегося буквой или цифрой
    - ИЛИ состоять из нескольких букв и/или цифр
    Т.е. имя оператора НЕ МОЖЕТ состоять из нескольких символов,
    не являющихся ни буквами, ни цифрами!
*/
const OpUnr op_unr_list[] =
{
    { "FICTIONAL",  NULL,           0,      NULL},
    { "+",          op_unr_plus,    1,      diff_op_plus},
    { "-",          op_unr_minus,   1,      diff_op_minus},
    { "sqrt",       op_unr_sqrt,    1,      diff_op_sqrt}
};
const OpBin op_bin_list[] =
{
    { "FICTIONAL",  NULL,           0,      NULL},
    { "+",          op_bin_add,     4,      diff_op_add},
    { "-",          op_bin_sub,     4,      diff_op_sub},
    { "*",          op_bin_mul,     3,      diff_op_mul},
    { "/",          op_bin_div,     3,      diff_op_div},
    { "^",          op_bin_pow,     2,      diff_op_pow}
};

enum OpsUnr
{
    OP_PLUS = 1,
    OP_MINUS,
    OP_SQRT,
};

enum OpsBin
{
    OP_ADD = 1,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
};


void diff_print_status_message( FILE *stream, DiffStatus status );

//! @brief Copies ExprNodeData from given data and returns it.
ExprNodeData diff_get_data( const TreeNode *node_ptr );

//! @brief Returns type of the given node.
ExprNodeType diff_get_type( const TreeNode *node_ptr );

TreeNode* diff_get_child( const Tree* tree_ptr, const TreeNode* node_ptr, Child child );


//! @brief Returns const, stored in given node.
double diff_get_const( const TreeNode *node_ptr );

//! @brief Writes given constant into given node, changing its type to const.
void diff_write_const( Tree *expr_tree, TreeNode *node_ptr, double cnst );

//! @brief Inserts a new node of type 'const' as the root of the tree.
void diff_insert_const_as_root( Tree *expr_tree, double cnst );

//! @brief Inserts a new node of type 'const' as the left child of the given node.
void diff_insert_const_at_left( Tree *expr_tree, TreeNode *node_ptr, double cnst );

//! @brief Inserts a new node of type 'const' as the right child of the given node.
void diff_insert_const_at_right( Tree *expr_tree, TreeNode *node_ptr, double cnst );

//! @brief Inserts a new node of type 'const' as the specified child of the given node.
void diff_insert_const( Tree *expr_tree, TreeNode *node_ptr, double cnst, Child child );


//! @brief Returns variable, stored in given node.
var_t diff_get_var( const TreeNode *node_ptr );

//! @brief Writes given variable into given node, changing its type to variable.
void diff_write_var( Tree *expr_tree, TreeNode *node_ptr, var_t var );

//! @brief Inserts a new node of type 'var' as the root of the tree.
void diff_insert_var_as_root( Tree *expr_tree, var_t var );

//! @brief Inserts a new node of type 'variable' as the left child of the given node.
void diff_insert_var_at_left( Tree *expr_tree, TreeNode *node_ptr, var_t var );

//! @brief Inserts a new node of type 'variable' as the right child of the given node.
void diff_insert_var_at_right( Tree *expr_tree, TreeNode *node_ptr, var_t var );

//! @brief Inserts a new node of type 'var' as the specified child of the given node.
void diff_insert_var( Tree *expr_tree, TreeNode *node_ptr, var_t var, Child child );


//! @brief Returns unary operator, stored in given node.
op_unr_t diff_get_op_unr( const TreeNode *node_ptr );

//! @brief Writes given unary operator into given node, changing its type to unary operator.
void diff_write_op_unr( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Inserts a new node of type 'op_unr' as the root of the tree.
void diff_insert_op_unr_as_root( Tree *expr_tree, op_unr_t op_unr );

//! @brief Inserts a new node of type 'unary operator' as the left child of the given node.
void diff_insert_op_unr_at_left( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Inserts a new node of type 'unary operator' as the right child of the given node.
void diff_insert_op_unr_at_right( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Inserts a new node of type 'op_unr' as the specified child of the given node.
void diff_insert_op_unr( Tree *expr_tree, TreeNode *node_ptr, op_unr_t op_unr, Child child );


//! @brief Returns binary operator, stored in given node.
op_bin_t diff_get_op_bin( const TreeNode *node_ptr );

//! @brief Writes given binary operator into given node, changing its type to binary operator.
void diff_write_op_bin( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin );

//! @brief Inserts a new node of type 'op_bin' as the root of the tree.
void diff_insert_op_bin_as_root( Tree *expr_tree, op_bin_t op_bin );

//! @brief Inserts a new node of type 'binary operator' as the left child of the given node.
void diff_insert_op_bin_at_left( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin );

//! @brief Inserts a new node of type 'binary operator' as the right child of the given node.
void diff_insert_op_bin_at_right( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin );

//! @brief Inserts a new node of type 'op_bin' as the specified child of the given node.
void diff_insert_op_bin( Tree *expr_tree, TreeNode *node_ptr, op_bin_t op_bin, Child child );

#define size_of_arr(arr) (sizeof(arr)/sizeof(*arr))

#define FREE(ptr) do { if (ptr) free(ptr); ptr = NULL; } while(0)

void expr_node_data_print( FILE* stream, void *data_ptr );

#endif /* DIFF_COMMON_H */
