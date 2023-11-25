#ifndef DIFF_COMMON_H
#define DIFF_COMMON_H

#include "..\..\..\mylibheaders\tree.h"
#include "op_funcs.h"


typedef uint8_t var_t;
typedef uint8_t op_unr_t;
typedef uint8_t op_bin_t;


#define DEF_DIFF_STATUS(name, message) DIFF_STATUS_##name,
enum DiffStatus
{
    #include "diff_statuses.h"
};
#undef DEF_DIFF_STATUS

//! @note Every variable is assigned an id, starting from zero. This very id
//! is used to get string, containing variable's name.
struct Expression
{
    Tree expr_tree  = {};   //< Expression itself.
    char **vars     = NULL; //< Contains variable's names.
    size_t vars_len = 0;    //< Length of 'vars' array.
};

enum ExprNodeType
{
    ERROR,
    CONST,
    VAR,
    OP_UNR,
    OP_BIN,
};

struct ExprNodeData
{
    ExprNodeType type = CONST;
    union
    {
        double cnst;
        var_t var;
        op_unr_t op_unr;
        op_bin_t op_bin;
    };
};

const size_t OP_NAME_LEN = 16;

struct OpUnr
{
    char name[OP_NAME_LEN] = "";
    double (*op_f)(double a) = NULL;
};

struct OpBin
{
    char name[OP_NAME_LEN] = "";
    double (*op_f)(double a, double b) = NULL;
};


const OpUnr op_unr_list[] =
{
    { "+",      op_unr_plus     },
    { "-",      op_unr_minus    },
    { "sqrt",   op_unr_sqrt     }
};
const OpBin op_bin_list[] =
{
    { "+",      op_bin_add      },
    { "-",      op_bin_sub      },
    { "*",      op_bin_mul      },
    { "/",      op_bin_div      },
    { "^",      op_bin_pow      }
};

#define DEF_DIFF_STATUS(name, message) message,
const char * const diff_status_messages[] =
{
    #include "diff_statuses.h"
    "FICTIONAL MESSAGE!"
};
#undef DEF_DIFF_STATUS


void diff_print_status_message( FILE *stream, DiffStatus status );

//! @brief Copies ExprNodeData from given data and returns it.
ExprNodeData diff_get_data( const TreeNode *node_ptr );

//! @brief Returns type of the given node.
ExprNodeType diff_get_type( const TreeNode *node_ptr );


//! @brief Returns const, stored in given node.
double diff_get_const( Expression *expr_ptr, TreeNode *node_ptr );

//! @brief Writes given constant into given node, changing its type to const.
void diff_write_const( Expression *expr_ptr, TreeNode *node_ptr, double cnst );

//! @brief Inserts a new node of type 'const' as the left child of the given node.
void diff_insert_const_at_left( Expression *expr_ptr, TreeNode *node_ptr, double cnst );

//! @brief Inserts a new node of type 'const' as the right child of the given node.
void diff_insert_const_at_right( Expression *expr_ptr, TreeNode *node_ptr, double cnst );


//! @brief Returns variable, stored in given node.
var_t diff_get_var( Expression *expr_ptr, TreeNode *node_ptr );

//! @brief Writes given variable into given node, changing its type to variable.
void diff_write_var( Expression *expr_ptr, TreeNode *node_ptr, var_t var );

//! @brief Inserts a new node of type 'variable' as the left child of the given node.
void diff_insert_var_at_left( Expression *expr_ptr, TreeNode *node_ptr, var_t var );

//! @brief Inserts a new node of type 'variable' as the right child of the given node.
void diff_insert_var_at_right( Expression *expr_ptr, TreeNode *node_ptr, var_t var );


//! @brief Returns unary operator, stored in given node.
op_unr_t diff_get_op_unr( Expression *expr_ptr, TreeNode *node_ptr );

//! @brief Writes given unary operator into given node, changing its type to unary operator.
void diff_write_op_unr( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Inserts a new node of type 'unary operator' as the left child of the given node.
void diff_insert_op_unr_at_left( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Inserts a new node of type 'unary operator' as the right child of the given node.
void diff_insert_op_unr_at_right( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Returns binary operator, stored in given node.
op_bin_t diff_get_op_bin( Expression *expr_ptr, TreeNode *node_ptr );

//! @brief Writes given binary operator into given node, changing its type to binary operator.
void diff_write_op_bin( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin );

//! @brief Inserts a new node of type 'binary operator' as the left child of the given node.
void diff_insert_op_bin_at_left( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin );

//! @brief Inserts a new node of type 'binary operator' as the right child of the given node.
void diff_insert_op_bin_at_right( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin );
#endif /* DIFF_COMMON_H */
