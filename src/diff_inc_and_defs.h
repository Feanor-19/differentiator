#ifndef DIFF_INC_AND_DEFS_H
#define DIFF_INC_AND_DEFS_H


#include "..\..\..\mylibheaders\tree.h"
#include "..\..\..\mylibheaders\onegin.h"


typedef uint8_t op_unr_t;
typedef uint8_t op_bin_t;
typedef uint8_t op_prior_t;
typedef uint8_t var_t;


enum Child
{
    ROOT,
    LEFT,
    RIGHT,
};

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


// TODO -
/*
typedef void (*diff_op_t)( const Tree *src_tree,
                           const TreeNode *diff_node,
                           Tree *new_tree,
                           TreeNode *parent_node_ptr,
                           Child child,
                           var_t diff_by );
*/
struct OpUnr
{
    char name[OP_NAME_MAX_LEN]  = "";   // Operator's name. SEE NOTE CONCERNING NAMES!
    double (*op_f)(double a)    = NULL; //< Pointer to function, associated with this operator.
    op_prior_t prior            = 0;    //< the lesser the number is, the higher priority.
    void (*diff_op)( const Tree *src_tree,
                     const TreeNode *diff_node,
                     Tree *new_tree,
                     TreeNode *parent_node_ptr,
                     Child child,
                     var_t diff_by ) = NULL; //< Pointer to function, which differetiates this operator.
};

struct OpBin
{
    char name[OP_NAME_MAX_LEN]          = "";   // Operator's name. SEE NOTE CONCERNING NAMES!
    double (*op_f)(double a, double b)  = NULL; //< Pointer to function, associated with this operator.
    op_prior_t prior                    = 0;    //< The lesser the number is, the higher priority.
    void (*diff_op)( const Tree *src_tree,
                     const TreeNode *diff_node,
                     Tree *new_tree,
                     TreeNode *parent_node_ptr,
                     Child child,
                     var_t diff_by ) = NULL; //< Pointer to function, which differetiates this operator.
};


#endif /* DIFF_INC_AND_DEFS_H */
