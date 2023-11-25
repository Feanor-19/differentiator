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
ExprNodeData diff_get_data( const Expression *expr_ptr, const TreeNode *node_ptr );

double diff_get_const( Expression *expr_ptr, TreeNode *node_ptr );

void diff_write_const( Expression *expr_ptr, TreeNode *node_ptr, double cnst );

#endif /* DIFF_COMMON_H */
