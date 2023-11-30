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

inline void write_char_to_token_buf( char *tokens_buf,
                                     size_t *tokens_buf_cap,
                                     size_t *tokens_buf_ind,
                                     char c )
{
    assert(tokens_buf);
    assert(tokens_buf_cap);
    assert(tokens_buf_ind);

    realloc_arr_if_needed((void**) &tokens_buf, tokens_buf_cap, *tokens_buf_ind, sizeof(char));

    tokens_buf[(*tokens_buf_ind)++] = c;
}

void print_dfa_error( char *file_buf, size_t err_ind)
{
    assert(file_buf);
    // печатает часть file_buf ДО err_ind (символов 20, но не вылезая за самое начало!!)
    // TODO -
}

char *my_strtok(char *tokens_buf,
                size_t *tokens_buf_cap,
                size_t *tokens_buf_ind,
                DiffStatus *err,
                char *file_buf = NULL)
{
    assert(tokens_buf);
    assert(tokens_buf_cap);
    assert(tokens_buf_ind);

    static char *file_buf_start = NULL;
    static char *file_buf_ptr   = NULL;
    if (file_buf)
    {
        // first run
        file_buf_ptr    = file_buf;
        file_buf_start  = file_buf;
    }

    DFAStates state = DFA_START;
    char *token_start = NULL;
    while (1)
    {
        switch (state)
        {
        case DFA_START:
            if ( *file_buf_ptr == ' ' )
            {
                file_buf_ptr++;
            }
            else if ( *file_buf_ptr == '\0' )
            {
                return NULL;
            }
            else
            {
                state = DFA_WORD;
                token_start = tokens_buf + *tokens_buf_ind;
                write_char_to_token_buf( tokens_buf, tokens_buf_cap, tokens_buf_ind, *file_buf_ptr );
                file_buf_ptr++;
            }
            break;
        case DFA_WORD:
            if ( *file_buf_ptr == ' ' || *file_buf_ptr == '\0' )
            {
                write_char_to_token_buf( tokens_buf, tokens_buf_cap, tokens_buf_ind, '\0');
                return token_start;
            }
            else
            {
                write_char_to_token_buf( tokens_buf, tokens_buf_cap, tokens_buf_ind, *file_buf_ptr );
                file_buf_ptr++;
            }
        default:
            assert(0 && "Unknown DKA state!");
            break;
        }
    }

}

DiffStatus parse_file_buf( FileBuf file_buf, ParsedFileBuf *ret )
{
    assert(file_buf.buf);
    assert(ret);

    size_t tokens_buf_cap = file_buf.buf_size;
    char *tokens_buf = (char *) calloc( tokens_buf_cap, sizeof(char) );
    size_t tokens_buf_ind = 0;

    size_t tokens_cap = TOKENS_DEFAULT_LEN;
    char **tokens = (char **) calloc( tokens_cap, sizeof(char*) );
    size_t tokens_ind = 0;

    DiffStatus err = DIFF_STATUS_OK;
    char *curr_token = my_strtok( tokens_buf, &tokens_buf_cap, &tokens_ind, &err, file_buf.buf );
    while (curr_token != NULL)
    {
        tokens[tokens_ind++] = curr_token;
        REALLOC_ARR_WRP(tokens, char*);

        curr_token = my_strtok(tokens_buf, &tokens_buf_cap, &tokens_ind, &err);
    }

    if (err)
        return DIFF_STATUS_ERROR_DFA;

    *ret = {tokens_buf, tokens, tokens_ind};
    return DIFF_STATUS_OK;

    /*
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
    */
}

void realloc_arr_if_needed( void **arr_ptr, size_t *arr_cap_ptr, size_t arr_ind, size_t elem_size )
{
    if ( arr_ind >= *arr_cap_ptr )
    {
        size_t new_cap = REALLOC_DEFAULT_MULTIPLIER * (*arr_cap_ptr);
        void *new_mem = (void*) realloc( *arr_ptr, new_cap*elem_size );
        if (!new_mem)
        {
            free(*arr_ptr);
            *arr_ptr = NULL;
        }
        *arr_ptr = new_mem;
        *arr_cap_ptr = new_cap;
    }
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

    for (op_unr_t op_id = 1; op_id < size_of_arr(op_unr_list); op_id++ )
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

    for (op_bin_t op_id = 1; op_id < size_of_arr(op_bin_list); op_id++ )
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
//! if yes, returns its sequantial number (id), if no, returns new sequential number and
//! sets '*is_new' to 1.
inline var_t get_var_id(    ParsedFileBuf parsed_buf,
                            const char* token,
                            VarForParsing *vars,
                            size_t vars_len,
                            int *is_new )
{
    var_t max_var_id = 0;
    for (var_t ind = 0; ind < vars_len; ind++)
    {
        if ( strcmp( parsed_buf.tokens[ vars[ind].token_ind ], token ) == 0 )
            return vars[ind].var_id;

        if ( vars[ind].var_id > max_var_id )
            max_var_id = vars[ind].var_id;
    }
    *is_new = 1;
    if (vars_len == 0)
        return 0;

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

DiffStatus diff_assemble_vars_ops_raw(ParsedFileBuf parsed_buf, VarsOpsRaw *ret)
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

    size_t vars_names_cap = VARS_DEFAULT_NUMBER;
    char **vars_names = (char**) calloc( vars_names_cap, sizeof(char*) );
    size_t vars_names_ind = 0;

    for (size_t ind = 0; ind < parsed_buf.n_tokens; ind++)
    {
        char *token = parsed_buf.tokens[ind];
        if ( token[0] == '(' || token[0] == ')' )
            continue;

        op_unr_t op_unr = 0;
        op_bin_t op_bin = 0;
        if ( (op_unr = check_is_token_op_unr(token) ) != 0 )
        {
            REALLOC_ARR_WRP(ops_unr, OpUnrForParsing);
            ops_unr[ops_unr_ind].op_unr_id = op_unr;
            ops_unr[ops_unr_ind].token_ind = ind;
            ops_unr_ind++;
        }

        if ( (op_bin = check_is_token_op_bin(token) ) != 0 )
        {
            REALLOC_ARR_WRP(ops_bin, OpBinForParsing);
            ops_bin[ops_bin_ind].op_bin_id = op_bin;
            ops_bin[ops_bin_ind].token_ind = ind;
            ops_bin_ind++;
        }

        if ( op_unr == 0 && op_bin == 0 && check_does_token_contain_only_letters(token) )
        {
            REALLOC_ARR_WRP(vars, VarForParsing);
            int is_new = 0;
            vars[vars_ind].var_id       = get_var_id( parsed_buf, token, vars, vars_ind, &is_new );
            vars[vars_ind].token_ind    = ind;
            vars_ind++;

            if (is_new)
            {
                REALLOC_ARR_WRP(vars_names, char*);
                vars_names[vars_names_ind++] = token;
            }
        }
    }

    *ret = { vars, vars_ind, ops_unr, ops_unr_ind, ops_bin, ops_bin_ind, vars_names, vars_names_ind };
    return DIFF_STATUS_OK;
}

DiffStatus diff_assemble_expr_tree( ParsedFileBuf parsed_buf, const VarsOpsRaw *raw_ptr, Tree *ret )
{
    assert(parsed_buf.tokens);
    assert(raw_ptr);
    assert(ret);

    Tree tree = {};
    tree_ctor(&tree, sizeof(ExprNodeData), NULL, expr_node_data_print);




    *ret = tree;

    return DIFF_STATUS_OK;
}

Expression diff_assemble_expression(    Tree *expr_tree,
                                        char** vars_names,
                                        size_t vars_names_len )
{
    assert(expr_tree);
    assert(vars_names);

    Expression expr     = {};

    expr.expr_tree      = *expr_tree;
    expr.vars_names_len = vars_names_len;
    expr.vars_names = (char**) calloc( vars_names_len, sizeof(char*) );
    for (size_t ind = 0; ind < vars_names_len; ind++)
    {
        expr.vars_names[ind] = strdup( vars_names[ind] );
    }

    return expr;
}

void parsed_file_buf_dtor( ParsedFileBuf *parsed_buf_ptr )
{
    if ( parsed_buf_ptr )
    {
        FREE(parsed_buf_ptr->tokens);
        FREE(parsed_buf_ptr->tokens_buf);
        parsed_buf_ptr->n_tokens = 0;
    }
}

void vars_ops_raw_dtor( VarsOpsRaw *vars_ops_raw_ptr )
{
    if ( vars_ops_raw_ptr )
    {
        FREE(vars_ops_raw_ptr->ops_bin_for_parsing);
        FREE(vars_ops_raw_ptr->ops_unr_for_parsing);
        FREE(vars_ops_raw_ptr->vars_for_parsing);
        FREE(vars_ops_raw_ptr->vars_names);
        vars_ops_raw_ptr->n_ops_bin     = 0;
        vars_ops_raw_ptr->n_ops_unr     = 0;
        vars_ops_raw_ptr->n_vars        = 0;
        vars_ops_raw_ptr->n_var_names   = 0;
    }
}
