#include "differentiator_input.h"

#include <assert.h>
#include <ctype.h>


//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_num( Token *tokens, size_t tokens_ind, double num )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_NUM;
    tokens[tokens_ind].num = num;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_var( Token *tokens, size_t tokens_ind, var_t var )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_VAR;
    tokens[tokens_ind].var = var;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_op_cmn( Token *tokens, size_t tokens_ind, TokenOpCmnName tkn_cmn_name )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_OP;
    tokens[tokens_ind].token_op.tkn_op_type = TKN_OP_TYPE_CMN;
    tokens[tokens_ind].token_op.op_cmn = tkn_cmn_name;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_op_unr( Token *tokens, size_t tokens_ind, op_unr_t tkn_unr )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_OP;
    tokens[tokens_ind].token_op.tkn_op_type = TKN_OP_TYPE_UNR;
    tokens[tokens_ind].token_op.op_unr = tkn_unr;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_op_bin( Token *tokens, size_t tokens_ind, op_bin_t tkn_bin )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_OP;
    tokens[tokens_ind].token_op.tkn_op_type = TKN_OP_TYPE_BIN;
    tokens[tokens_ind].token_op.op_bin = tkn_bin;
}

DiffStatus parse_file_buf( FileStream file, TknsAndVars *ret )
{
    assert(file.str);
    assert(ret);

    size_t tokens_cap = TOKENS_DEFAULT_LEN;
    Token *tokens = (Token *) calloc( tokens_cap, sizeof(Token) );
    size_t tokens_ind = 0;

    size_t vars_names_cap = VARS_DEFAULT_NUMBER;
    char **vars_names = (char**) calloc( vars_names_cap, sizeof(char*) );
    size_t vars_names_ind = 0;

    op_unr_t op_unr = 0;
    op_bin_t op_bin = 0;

    while( (size_t) (file.ptr - file.str) < file.size + 1 )
    {
        if ( isspace(*file.ptr) || *file.ptr == '\n' )
        {
            file.ptr++;
            continue;
        }

        if ( *file.ptr == '(' || *file.ptr == ')' )
        {
            REALLOC_ARR_WRP(tokens, Token);
            if ( *file.ptr == '(' )
                token_put_op_cmn( tokens, tokens_ind, TKN_OP_OPN_BRACKET );
            else
                token_put_op_cmn( tokens, tokens_ind, TKN_OP_CLS_BRACKET );
            tokens_ind++;
            file.ptr++;
        }
        else if ( *file.ptr == '\0')
        {
            REALLOC_ARR_WRP(tokens, Token);
            token_put_op_cmn( tokens, tokens_ind, TKN_OP_END );
            tokens_ind++;
            file.ptr++;
        }
        else if ( isdigit( *file.ptr ) )
        {
            double num = NAN;
            int read_chars = 0;
            int is_num_read = sscanf(file.ptr, "%lf%n", &num, &read_chars);
            if (!is_num_read)
            {
                // TODO - ОШИБКА!
            }

            file.ptr += read_chars;

            REALLOC_ARR_WRP(tokens, Token);
            token_put_num( tokens, tokens_ind, num );
            tokens_ind++;
        }
        else if ( (op_unr = check_is_op_unr(&file) ) != 0 )
        {
            REALLOC_ARR_WRP(tokens, Token);
            token_put_op_unr( tokens, tokens_ind, op_unr );
            tokens_ind++;
        }
        else if ( (op_bin = check_is_op_bin(&file) ) != 0 )
        {
            REALLOC_ARR_WRP(tokens, Token);
            token_put_op_bin( tokens, tokens_ind, op_bin );
            tokens_ind++;
        }
        else
        {
            int is_new = 0;
            var_t var = get_var_id( &file, vars_names, vars_names_ind, &is_new );
            if (is_new)
            {
                vars_names_ind++;
                REALLOC_ARR_WRP(vars_names, char*);
            }

            REALLOC_ARR_WRP(tokens, Token);
            token_put_var( tokens, tokens_ind, var );
            tokens_ind++;
        }
    }


    *ret = {tokens, tokens_ind, vars_names, vars_names_ind};
    return DIFF_STATUS_OK;
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

/*
int check_does_token_contain_only_letters( const char *token )
{
    assert(token);

    while ( *token != '\0' && isalpha(*token) )
        token++;

    if (*token != '\0')
        return 0;

    return 1;
}
*/

inline int is_char_allowed_in_identifiers( char c )
{
    return (isalnum(c) || c == '_');
}

inline size_t find_identifier_end( const char* str )
{
    assert(str);
    const char *start = str;
    while ( is_char_allowed_in_identifiers(*str) && (*str != '\0') )
        str++;

    return str - start;
}

op_unr_t check_is_op_unr( FileStream *file )
{
    assert(file);

    size_t off = 0;
    if ( isalnum( *file->ptr ) )
        off = find_identifier_end( file->ptr );
    else
        off = 1;
    char tmp = file->ptr[off];
    file->ptr[off] = '\0';

    for (op_unr_t op_id = 1; op_id < size_of_arr(op_unr_list); op_id++ )
    {
        if ( strcmp( file->ptr, op_unr_list[op_id].name ) == 0 )
        {
            file->ptr[off] = tmp;
            if ( op_id == (op_unr_t) OP_MINUS && !isalnum(tmp) )
                return 0; // this is not unary minus! this is a binary one!
            if ( op_id == (op_unr_t) OP_PLUS  && !isalnum(tmp) )
                return 0; // this is not unary plus!  this is a binary one!

            file->ptr += off;
            return op_id;
        }
    }

    file->ptr[off] = tmp;
    return 0;
}

op_bin_t check_is_op_bin( FileStream *file )
{
    assert(file);

    size_t off = 0;
    if ( isalnum( *file->ptr ) )
        off = find_identifier_end( file->ptr );
    else
        off = 1;
    char tmp = file->ptr[off];
    file->ptr[off] = '\0';

    for (op_bin_t op_id = 1; op_id < size_of_arr(op_bin_list); op_id++ )
    {
        if ( strcmp( file->ptr, op_bin_list[op_id].name ) == 0 )
        {
            file->ptr[off] = tmp;
            file->ptr += off;
            return op_id;
        }
    }

    file->ptr[off] = tmp;
    return 0;
}

//! @brief Receieves token, which DEFINITELY contains a variable,
//! and array of variable's names vars_names. Determines whether this variable was met earlier or not;
//! if yes, returns its sequantial number (id), if no, returns new sequential number and
//! sets '*is_new' to 1.
//! @note DO REALLOC OF VARS_NAMES AFTER THIS!
var_t get_var_id( FileStream *file,
                  char **vars_names,
                  size_t vars_names_ind,
                  int *is_new)
{
    assert(file);
    assert(vars_names);
    assert(is_new);

    size_t off = find_identifier_end( file->ptr );
    char tmp = file->ptr[off];
    file->ptr[off] = '\0';

    for (var_t ind = 0; ind < vars_names_ind; ind++)
    {
        if ( strcmp( vars_names[ind], file->ptr ) == 0 )
        {
            file->ptr[off] = tmp;
            file->ptr += off;
            return ind;
        }
    }

    // adding new var name
    vars_names[vars_names_ind] = strdup( file->ptr );
    *is_new = 1;

    file->ptr[off] = tmp;
    file->ptr += off;
    return (var_t) vars_names_ind;
}

void diff_get_addive( TknsAndVars *parsed_tokens, Tree *tree, TreeNode *prnt_node, Child child )
{

}

DiffStatus diff_assemble_expr_tree( TknsAndVars *parsed_tokens, Tree *expr_tree )
{
    assert(parsed_tokens);
    assert(parsed_tokens->tokens);
    assert(expr_tree);

    tree_ctor(expr_tree, sizeof(ExprNodeData), NULL, expr_node_data_print);

    diff_get_addive( parsed_tokens, expr_tree, NULL, ROOT );



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

void parsed_tokens_and_vars_dtor( TknsAndVars *parsed_tokens )
{
    if ( parsed_tokens )
    {
        FREE(parsed_tokens->tokens);
        parsed_tokens->n_tokens = 0;

        for (size_t ind = 0; ind < parsed_tokens->n_vars; ind++)
        {
            FREE(parsed_tokens->vars_names[ind]);
        }
        FREE(parsed_tokens->vars_names);
        parsed_tokens->n_vars = 0;
    }
}
