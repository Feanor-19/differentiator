#ifndef DIFF_INPUT_H
#define DIFF_INPUT_H

#include "differentiator_common.h"


struct ParsedFileBuf
{
    char **tokens   = NULL;
    size_t n_tokens = 0;
};

struct VarForParsing
{
    size_t token_ind    = 0;    //< Index of the token with the variable.
    var_t var_id       = 0;     //< Id of the variable in the mentioned token.
};

struct OpUnrForParsing
{
    size_t token_ind    = 0;    //< Index of the token with the unary operator.
    op_unr_t op_unr_id  = 0;    //< Id of the unary operator in the mentioned token.
};

struct OpBinForParsing
{
    size_t token_ind    = 0;    //< Index of the token with the binary operator.
    op_bin_t op_bin_id  = 0;    //< Id of the binary operator in the mentioned token.
};

struct VarsOpsRaw
{
    struct
    {
        VarForParsing *vars_for_parsing         = NULL;
        size_t n_vars                           = 0;
    };
    struct
    {
        OpUnrForParsing *ops_unr_for_parsing    = NULL;
        size_t n_ops_unr                        = 0;
    };
    struct
    {
        OpBinForParsing *ops_bin_for_parsing    = NULL;
        size_t n_ops_bin                        = 0;
    };
    struct
    {
        char **vars_names                       = NULL;
        size_t n_var_names                      = 0;
    };

};


const size_t REALLOC_DEFAULT_MULTIPLIER = 2;
const size_t TOKENS_DEFAULT_LEN         = 10;
const size_t VARS_DEFAULT_NUMBER        = 1;
const size_t OPS_UNR_DEFAULT_NUMBER     = 10;
const size_t OPS_BIN_DEFAULT_NUMBER     = 5;


//! @brief Detaches all non letters and non digits, separating with spaces.
void preprocess_detach_not_alnums(FileBuf *file_buf_ptr);

DiffStatus parse_file_buf( FileBuf file_buf, ParsedFileBuf *ret );

void realloc_arr_if_needed( void **arr, size_t *arr_cap_ptr, size_t arr_ind, size_t elem_size );

#define REALLOC_ARR_WRP(arr, elem_t) do {                                       \
    realloc_arr_if_needed( (void**) &arr, &arr##_cap, arr##_ind, sizeof(elem_t) );       \
    if (!arr)                                                                   \
        return DIFF_STATUS_ERROR_MEM_ALLOC;                                     \
} while (0)

//! @brief Returns 1 if token contains olny letters, 0 otherwise.
int check_does_token_contain_only_letters( const char *token );

//! @brief Returns 0 if token isn't an unary operator, and unary operator's id otherwise
op_unr_t check_is_token_op_unr( const char *token );

//! @brief Returns 0 if token isn't an binary operator, and binary operator's id otherwise
op_bin_t check_is_token_op_bin( const char *token );

DiffStatus diff_get_vars_ops_raw(ParsedFileBuf parsed_buf, VarsOpsRaw *ret);


#endif /* DIFF_INPUT_H */
