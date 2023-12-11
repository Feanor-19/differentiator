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
    // TODO -
    ПРИМЕЧАНИЕ ОБ ИМЕНАХ ОПЕРАТОРОВ:
    Имя оператора может:
    - ИЛИ состоять из одного символа, не являющегося буквой или цифрой
    - ИЛИ состоять из нескольких букв и/или цифр
    Т.е. имя оператора НЕ МОЖЕТ состоять из нескольких символов,
    не являющихся ни буквами, ни цифрами!

    ПРИМЕЧАНИЕ ОБ УНАРНОМ МИНУСЕ:
    - минус считается унарным только если он стоит сразу перед цифрой или буквой, без пробелов.
*/
const OpUnr op_unr_list[] =
{
    { "FICTIONAL",  TKN_OP_UNR_DEFAULT,     NULL,           NULL},
    { "+",          TKN_OP_UNR_DEFAULT,     op_unr_plus,    diff_op_plus},
    { "-",          TKN_OP_UNR_DEFAULT,     op_unr_minus,   diff_op_minus},
    { "sqrt",       TKN_OP_UNR_DEFAULT,     op_unr_sqrt,    diff_op_sqrt},
    { "exp",        TKN_OP_UNR_DEFAULT,     op_unr_exp,     diff_op_exp},
    { "ln",         TKN_OP_UNR_DEFAULT,     op_unr_ln,      diff_op_ln},
    { "sin",        TKN_OP_UNR_DEFAULT,     op_unr_sin,     diff_op_sin},
    { "cos",        TKN_OP_UNR_DEFAULT,     op_unr_cos,     diff_op_cos}
};
const OpBin op_bin_list[] =
{
    { "FICTIONAL",  TKN_OP_BIN_GROUP_ADD,   NULL,           NULL},
    { "+",          TKN_OP_BIN_GROUP_ADD,   op_bin_add,     diff_op_add},
    { "-",          TKN_OP_BIN_GROUP_ADD,   op_bin_sub,     diff_op_sub},
    { "*",          TKN_OP_BIN_GROUP_MUL,   op_bin_mul,     diff_op_mul},
    { "/",          TKN_OP_BIN_GROUP_MUL,   op_bin_div,     diff_op_div},
    { "^",          TKN_OP_BIN_GROUP_POW,   op_bin_pow,     diff_op_pow}
};

enum OpsUnr
{
    OP_PLUS = 1,
    OP_MINUS,
    OP_SQRT,
    OP_EXP,
    OP_LN,
    OP_SIN,
    OP_COS,
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

//! @brief Creates a new node using 'op_new_TreeNode()' and writes given const into it.
TreeNode *diff_new_const_node( Tree *expr_tree, double cnst );


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

//! @brief Creates a new node using 'op_new_TreeNode()' and writes given var into it.
TreeNode *diff_new_var_node( Tree *expr_tree, var_t var );


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

//! @brief Creates a new node using 'op_new_TreeNode()' and writes given op_unr into it.
TreeNode *diff_new_op_unr_node( Tree *expr_tree, op_unr_t op_unr );


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

//! @brief Creates a new node using 'op_new_TreeNode()' and writes given op_bin into it.
TreeNode *diff_new_op_bin_node( Tree *expr_tree, op_bin_t op_bin );



#define size_of_arr(arr) (sizeof(arr)/sizeof(*arr))

#define FREE(ptr) do { if (ptr) free(ptr); ptr = NULL; } while(0)

void expr_node_data_print( FILE* stream, void *data_ptr );


#endif /* DIFF_COMMON_H */
