#include "differentiator_input.h"

#include <assert.h>
#include <ctype.h>

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

DiffStatus parse_file_buf( FileBuf file_buf, ParsedFileBuf *ret )
{
    assert(file_buf.buf);
    assert(ret);

    const char delim[] = {' ', '\n', '\0'};

    size_t tokens_cap = TOKENS_DEFAULT_LEN;
    char **tokens = (char **) calloc( tokens_cap, sizeof(char*) );
    size_t tokens_ind = 0;

    char *curr_token = strtok( file_buf.buf, delim );
    while (curr_token != NULL)
    {
        tokens[tokens_ind++] = curr_token;
        REALLOC_ARR_WRP(tokens, char*);

        curr_token = strtok( NULL, delim );
    }

    *ret = {tokens, tokens_ind};
    return DIFF_STATUS_OK;
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

int check_does_token_contain_only_letters( const char *token )
{
    assert(token);

    while ( *token != '\0' && isalpha(*token) )
        token++;

    if (*token != '\0')
        return 0;

    return 1;
}

op_unr_t check_is_token_op_unr( const char *token )
{
    assert(token);

    for (size_t op_id = 1; op_id < size_of_arr(op_unr_list); op_id++ )
    {
        if ( strcmp( token, op_unr_list[op_id].name ) == 0 )
        {
            return op_id;
        }
    }
    return 0;
}

op_bin_t check_is_token_op_bin( const char *token )
{
    assert(token);

    for (size_t op_id = 1; op_id < size_of_arr(op_bin_list); op_id++ )
    {
        if ( strcmp( token, op_bin_list[op_id].name ) == 0 )
        {
            return op_id;
        }
    }
    return 0;
}

//! @brief Receieves token, which DEFINITELY contains a variable,
//! and array of structs VarForParsing. Determines whether this variable was met earlier or not;
//! if yes, returns its sequantial number (id), if no, returns new sequential number.
inline var_t get_var_id(   ParsedFileBuf parsed_buf,
                                const char* token,
                                VarForParsing *vars,
                                size_t vars_len )
{
    size_t max_var_id = 0;
    for (size_t ind = 0; ind < vars_len; ind++)
    {
        if ( strcmp( parsed_buf.tokens[ vars[ind].token_ind ], token ) == 0 )
            return vars[ind].var_id;

        if ( vars[ind].var_id > max_var_id )
            max_var_id = vars[ind].var_id;
    }
    return max_var_id + 1;
}

/*
    ИДЕЯ: до основного прохода по токенам находим переменные,
    присваиваем им порядковые номера (начиная с нуля), а в специальный
    динамический массив записываем пары (индекс токена с переменной, порядковый номер переменной)

    а во время основного прохода будем потихоньку продвигаться по этому динамическому массиву,
    и если индекса нашего токена совпадает, то значит этот токен - переменная и мы знаем уже ее
    порядковый номер, который и надо писать в дерево
*/

DiffStatus diff_get_vars_ops_raw(ParsedFileBuf parsed_buf, VarsOpsRaw *ret)
{
    assert(parsed_buf.tokens);
    assert(ret);

    size_t vars_cap = VARS_DEFAULT_NUMBER;
    VarForParsing *vars = (VarForParsing*) calloc( vars_cap, sizeof(VarForParsing) );
    size_t vars_ind = 0;

    size_t ops_unr_cap = OPS_UNR_DEFAULT_NUMBER;
    OpUnrForParsing *ops_unr = (OpUnrForParsing*) calloc( ops_unr_cap, sizeof(OpUnrForParsing) );
    size_t ops_unr_ind = 0;

    size_t ops_bin_cap = OPS_BIN_DEFAULT_NUMBER;
    OpBinForParsing *ops_bin = (OpBinForParsing*) calloc( ops_bin_cap, sizeof(OpBinForParsing) );
    size_t ops_bin_ind = 0;

    for (size_t ind = 0; ind < parsed_buf.n_tokens; ind++)
    {
        char *token = parsed_buf.tokens[ind];
        if ( token[0] == '(' || token[0] == ')' )
            continue;

        op_unr_t op_unr = 0;
        op_bin_t op_bin = 0;
        if ( (op_unr = check_is_token_op_unr(token) ) != 0 )
        {
            ops_unr[ops_unr_ind].op_unr_id = op_unr;
            ops_unr[ops_unr_ind].token_ind = ind;
            ops_unr_ind++;
            REALLOC_ARR_WRP(ops_unr, OpUnrForParsing);
        }
        else if ( (op_bin = check_is_token_op_bin(token) ) != 0 )
        {
            ops_bin[ops_bin_ind].op_bin_id = op_bin;
            ops_bin[ops_bin_ind].token_ind = ind;
            ops_bin_ind++;
            REALLOC_ARR_WRP(ops_bin, OpBinForParsing);
        }
        else if ( check_does_token_contain_only_letters(token) )
        {
            vars[vars_ind].var_id       = get_var_id( parsed_buf, token, vars, vars_ind );
            vars[vars_ind].token_ind    = ind;
            vars_ind++;
            REALLOC_ARR_WRP(vars, VarForParsing);
        }
    }

    *ret = { vars, vars_ind, ops_unr, ops_unr_ind, ops_bin, ops_bin_ind };
    return DIFF_STATUS_OK;
}
