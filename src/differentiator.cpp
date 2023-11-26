#include "differentiator.h"

#include <assert.h>
#include <ctype.h>

void diff_expr_dtor( Expression *expr_ptr )
{
    assert(expr_ptr);

    tree_dtor(&expr_ptr->expr_tree);

    for (size_t ind = 0; ind < expr_ptr->vars.vars_len; ind++)
    {
        assert(expr_ptr->vars.vars_names[ind]);
        free(expr_ptr->vars.vars_names[ind]);
    }
}

void preprocess_detach_not_alnums(FileBuf *file_buf_ptr)
{
    assert(file_buf_ptr);
    assert(file_buf_ptr->buf);

    char *new_file_buf = (char*) calloc( file_buf_ptr->buf_size * 3, sizeof(char) );

    size_t buf_ind = 0;
    size_t new_buf_ind = 0;
    while (buf_ind < file_buf_ptr->buf_size)
    {
        if ( !isalnum( file_buf_ptr->buf[buf_ind] ) )
        {
            new_file_buf[new_buf_ind++] = ' ';
            new_file_buf[new_buf_ind++] = file_buf_ptr->buf[buf_ind];
            new_file_buf[new_buf_ind++] = ' ';
        }
        else
        {
            new_file_buf[new_buf_ind++] = file_buf_ptr->buf[buf_ind];
        }
        buf_ind++;
    }

    free(file_buf_ptr->buf);

    file_buf_ptr->buf = new_file_buf;
    file_buf_ptr->buf_size = new_buf_ind;
}

ParsedFileBuf parse_file_buf( FileBuf file_buf )
{
    assert(file_buf.buf);

    const char delim[] = {' ', '\n', '\0'};

    size_t tokens_cap = TOKENS_DEFAULT_LEN;
    char **tokens = (char **) calloc( tokens_cap, sizeof(char*) );
    size_t tokens_ind = 0;

    char *curr_token = strtok( file_buf.buf, delim );
    while (curr_token != NULL)
    {
        tokens[tokens_ind++] = curr_token;
        tokens = (char**) realloc_arr_if_needed(tokens, &tokens_cap, tokens_ind, sizeof(char*));
        if (!tokens)
            return {NULL, 0};

        curr_token = strtok( NULL, delim );
    }

    return {tokens, tokens_ind};
}

void *realloc_arr_if_needed( void *arr, size_t *arr_cap_ptr, size_t vars_ind, size_t elem_size )
{
    if ( vars_ind >= *arr_cap_ptr )
    {
        size_t new_cap = REALLOC_DEFAULT_MULTIPLIER * (*arr_cap_ptr);
        void *new_mem = (void*) realloc( arr, new_cap*elem_size );
        if (!new_mem)
        {
            free(arr);
            return NULL;
        }
        arr = new_mem;
        *arr_cap_ptr = new_cap;
    }
    return arr;
}

// inline int check_is_str_op_unr( char *str )
// {
//     assert(str);

//     for (size_t ind = 0; ind < size_of_arr(op_unr_list); ind++)
//     {
//         if ( strncmp(str, op_unr_list[ind].name, OP_NAME_MAX_LEN) == 0 )
//             return 1;
//     }
//     return 0;
// }

// inline int check_is_str_op_bin( char *str )
// {
//     assert(str);

//     for (size_t ind = 0; ind < size_of_arr(op_bin_list); ind++)
//     {
//         if ( strncmp(str, op_bin_list[ind].name, OP_NAME_MAX_LEN) == 0 )
//             return 1;
//     }
//     return 0;
// }

// //! @brief Reads chars starting from 'start_ptr' in given 'buf'.
// //! If read word can't be a var's name, returns 0 and stops reading.
// //! If read word is too long, returns -1. Otherwise returns length of read word
// //! and read word is stored in the 'buf'.
// inline int check_is_word_var( char *start_ptr, char buf[] )
// {
//     assert(start_ptr);
//     assert(buf);

//     if (start)

//     char *ptr = start_ptr;
//     size_t buf_ind = 0;
//     while ( *ptr != ' ' && *ptr != '(' && *ptr != ')' && buf_ind < VAR_NAME_MAX_LEN - 1 )
//     {
//         buf[buf_ind++] = *(ptr++);
//     }

//     if (buf_ind >= VAR_NAME_MAX_LEN - 1)
//         return -1;

//     buf[buf_ind] = '\0';

//     if (!check_is_str_op_unr(buf) && !check_is_str_op_bin(buf) )
//         return buf_ind;

//     return 0;
// }



// Vars diff_get_vars(FileBuf file_buf)
// {
//     assert(file_buf.buf);

//     size_t file_ind = 0;
//     size_t vars_ind = 0;
//     size_t vars_cap = 1;
//     char **vars = (char**) calloc(vars_cap, sizeof(char*));
//     char tmp_buf[VAR_NAME_MAX_LEN];
//     while (file_ind < file_buf.buf_size)
//     {
//         int res = check_is_word_var( file_buf.buf + file_ind, tmp_buf );
//         if (res == -1)
//         {
//             // TODO - печать ошибки?
//             return {NULL, 0};
//         }
//         else if (res != 0)
//         {
//             char *new_var = (char*) calloc( res + 1, sizeof(char) );
//             strncpy(new_var, tmp_buf, res + 1);
//             vars[vars_ind++] = new_var;
//             if ( !realloc_vars_if_needed( &vars, &vars_cap, vars_ind ) )
//             {
//                 // TODO mem err
//                 return {NULL, 0};
//             }
//         }
//     }
//     return {vars, vars_ind};
// }
