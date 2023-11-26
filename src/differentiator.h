#ifndef DIFF_H
#define DIFF_H

#include "differentiator_common.h"


struct ParsedFileBuf
{
    char **tokens   = NULL;
    size_t n_tokens = 0;
};


const size_t REALLOC_DEFAULT_MULTIPLIER = 2;
const size_t TOKENS_DEFAULT_LEN         = 10;


void diff_expr_dtor( Expression *expr_ptr );

//! @brief Detaches all non letters and non digits, separating with spaces.
void preprocess_detach_not_alnums(FileBuf *file_buf_ptr);

ParsedFileBuf parse_file_buf( FileBuf file_buf );

void *realloc_arr_if_needed( void *arr, size_t *arr_cap_ptr, size_t vars_ind, size_t elem_size );

// TODO - в именах переменных можно использовать только буквы!!!

// Vars diff_get_vars(FileBuf file_buf);

#endif /* DIFF_H */
