#ifndef DIFF_COMMON_H
#define DIFF_COMMON_H

#include "..\..\..\mylibheaders\tree.h"
#include "..\..\..\mylibheaders\onegin.h"
#include "op_funcs.h"


typedef uint8_t var_t;
typedef uint8_t op_unr_t;
typedef uint8_t op_bin_t;
typedef uint8_t op_prior_t;


#define DEF_DIFF_STATUS(name, message) DIFF_STATUS_##name,
enum DiffStatus
{
    #include "diff_statuses.h"
};
#undef DEF_DIFF_STATUS

//! @note Every variable is assigned an id, starting from zero. This very id
//! is used to get string, containing variable's name.
//! @note VARIABLE'S NAMES CONTAIN ONLY LETTERS!
struct Expression
{
    Tree expr_tree                  = {};   //< Expression itself.
    char **vars_names  = NULL; //< Contains variable's names.
    size_t vars_names_len           = 0;    //< Length of 'vars' array.
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

const size_t OP_NAME_MAX_LEN    = 16;
const size_t VAR_NAME_MAX_LEN   = 32;
const size_t WORD_MAX_LEN       = 32;
static_assert( OP_NAME_MAX_LEN <= WORD_MAX_LEN && VAR_NAME_MAX_LEN <= WORD_MAX_LEN );

struct OpUnr
{
    char name[OP_NAME_MAX_LEN]  = "";   // Operator's name. SEE NOTE CONCERNING NAMES!
    double (*op_f)(double a)    = NULL; //< Function pointer.
    op_prior_t prior            = 0;    //< the lesser the number is, the higher priority.
};

struct OpBin
{
    char name[OP_NAME_MAX_LEN]          = "";   // Operator's name. SEE NOTE CONCERNING NAMES!
    double (*op_f)(double a, double b)  = NULL; //< Function pointer.
    op_prior_t prior                    = 0;    //< The lesser the number is, the higher priority.
};

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
    { "FICTIONAL",  NULL,           0},
    { "+",          op_unr_plus,    1},
    { "-",          op_unr_minus,   1},
    { "sqrt",       op_unr_sqrt,    1}
};
const OpBin op_bin_list[] =
{
    { "FICTIONAL",  NULL,           0},
    { "+",          op_bin_add,     4},
    { "-",          op_bin_sub,     4},
    { "*",          op_bin_mul,     3},
    { "/",          op_bin_div,     3},
    { "^",          op_bin_pow,     2}
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
double diff_get_const( TreeNode *node_ptr );

//! @brief Writes given constant into given node, changing its type to const.
void diff_write_const( Expression *expr_ptr, TreeNode *node_ptr, double cnst );

//! @brief Inserts a new node of type 'const' as the left child of the given node.
void diff_insert_const_at_left( Expression *expr_ptr, TreeNode *node_ptr, double cnst );

//! @brief Inserts a new node of type 'const' as the right child of the given node.
void diff_insert_const_at_right( Expression *expr_ptr, TreeNode *node_ptr, double cnst );


//! @brief Returns variable, stored in given node.
var_t diff_get_var( TreeNode *node_ptr );

//! @brief Writes given variable into given node, changing its type to variable.
void diff_write_var( Expression *expr_ptr, TreeNode *node_ptr, var_t var );

//! @brief Inserts a new node of type 'variable' as the left child of the given node.
void diff_insert_var_at_left( Expression *expr_ptr, TreeNode *node_ptr, var_t var );

//! @brief Inserts a new node of type 'variable' as the right child of the given node.
void diff_insert_var_at_right( Expression *expr_ptr, TreeNode *node_ptr, var_t var );


//! @brief Returns unary operator, stored in given node.
op_unr_t diff_get_op_unr( TreeNode *node_ptr );

//! @brief Writes given unary operator into given node, changing its type to unary operator.
void diff_write_op_unr( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Inserts a new node of type 'unary operator' as the left child of the given node.
void diff_insert_op_unr_at_left( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Inserts a new node of type 'unary operator' as the right child of the given node.
void diff_insert_op_unr_at_right( Expression *expr_ptr, TreeNode *node_ptr, op_unr_t op_unr );

//! @brief Returns binary operator, stored in given node.
op_bin_t diff_get_op_bin( TreeNode *node_ptr );

//! @brief Writes given binary operator into given node, changing its type to binary operator.
void diff_write_op_bin( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin );

//! @brief Inserts a new node of type 'binary operator' as the left child of the given node.
void diff_insert_op_bin_at_left( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin );

//! @brief Inserts a new node of type 'binary operator' as the right child of the given node.
void diff_insert_op_bin_at_right( Expression *expr_ptr, TreeNode *node_ptr, op_bin_t op_bin );

#define size_of_arr(arr) (sizeof(arr)/sizeof(*arr))

#define FREE(ptr) do { if (ptr) free(ptr); ptr = NULL; } while(0)

void expr_node_data_print( FILE* stream, void *data_ptr );

#endif /* DIFF_COMMON_H */
