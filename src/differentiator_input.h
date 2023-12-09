#ifndef DIFF_INPUT_H
#define DIFF_INPUT_H

#include "differentiator_common.h"


struct FileStream
{
    char *str   = NULL; //< The file as a string.
    size_t size = 0;    //< Number of chars in 'str', including '\0'.
    char *ptr   = NULL; //< Pointer to the current char.
};

enum TokenType
{
    TKN_TYPE_NUM,
    TKN_TYPE_VAR,
    TKN_TYPE_OP,
};

enum TokenOpCmnName
{
    TKN_OP_OPN_BRACKET,
    TKN_OP_CLS_BRACKET,
    TKN_OP_END,
};

enum TokenOpType
{
    TKN_OP_TYPE_CMN, //!< Opening and closing brackets, operator 'END', etc.
    TKN_OP_TYPE_UNR, //!< Tokens, which are associated with unary operators in the tree.
    TKN_OP_TYPE_BIN, //!< Tokens, which are associated with binary operators in the tree.
};

struct TokenOp
{
    TokenOpType tkn_op_type;
    union
    {
        TokenOpCmnName  op_cmn; //!< Name of the common token, used in this token.
        op_unr_t        op_unr; //!< Id of the unary operator used in this token, as used in the tree.
        op_bin_t        op_bin; //!< Id of the binary operator used in this token, as used in the tree.
    };
};

struct Token
{
    TokenType token_type;
    union
    {
        double num;
        var_t var;
        TokenOp token_op;
    };
};

struct TknsAndVars
{
    Token *tokens       = NULL;
    size_t n_tokens     = 0;
    char **vars_names   = NULL;
    size_t n_vars       = 0;
    size_t curr_tkn_ind = 0;
};


const size_t REALLOC_DEFAULT_MULTIPLIER = 2;
const size_t TOKENS_DEFAULT_LEN         = 10;
const size_t VARS_DEFAULT_NUMBER        = 2;


DiffStatus parse_file_buf( FileStream file, TknsAndVars *ret );

void realloc_arr_if_needed( void **arr, size_t *arr_cap_ptr, size_t arr_ind, size_t elem_size );

#define REALLOC_ARR_WRP(arr, elem_t) do {                                               \
    realloc_arr_if_needed( (void**) &arr, &arr##_cap, arr##_ind, sizeof(elem_t) );      \
    if (!arr)                                                                           \
        return DIFF_STATUS_ERROR_MEM_ALLOC;                                             \
} while (0)

//! @brief Returns 1 if token contains olny letters, 0 otherwise.
// int check_does_token_contain_only_letters( const char *token );

//! @brief Returns 0 if token isn't an unary operator, and unary operator's id otherwise
op_unr_t check_is_op_unr( FileStream *file );

//! @brief Returns 0 if token isn't an binary operator, and binary operator's id otherwise
op_bin_t check_is_op_bin( FileStream *file );

//! @brief Receieves token, which DEFINITELY contains a variable,
//! and array of variable's names vars_names. Determines whether this variable was met earlier or not;
//! if yes, returns its sequantial number (id), if no, returns new sequential number and
//! sets '*is_new' to 1.
//! @note DO REALLOC OF VARS_NAMES AFTER THIS!
var_t get_var_id( FileStream *file,
                  char **vars_names,
                  size_t vars_names_ind,
                  int *is_new);

//TODO - добавить описание после написания ф-ии
DiffStatus diff_assemble_expr_tree( TknsAndVars *parsed_tokens, Tree *expr_tree );

//! @brief Assembles Expression. MAKES A COPY OF vars_names, because otherwise it would have
//! pointers to memory, allocated for ParsedFileBuf and VarsOpsRaw, which must be deconstructed right
//! after the call of this function.
Expression diff_assemble_expression(    Tree *expr_tree,
                                        char** vars_names,
                                        size_t vars_names_len );

void parsed_tokens_and_vars_dtor( TknsAndVars *parsed_tokens );

#endif /* DIFF_INPUT_H */
