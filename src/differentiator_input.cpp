#include "differentiator_input.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>


//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_num( Token *tokens, size_t tokens_ind, char *tkn_start, double num )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_NUM;
    tokens[tokens_ind].num = num;
    tokens[tokens_ind].tkn_start = tkn_start;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_var( Token *tokens, size_t tokens_ind, char *tkn_start, var_t var )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_VAR;
    tokens[tokens_ind].var = var;
    tokens[tokens_ind].tkn_start = tkn_start;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_op_cmn( Token *tokens, size_t tokens_ind, char *tkn_start, TokenOpCmnName tkn_cmn_name )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_OP;
    tokens[tokens_ind].token_op.tkn_op_type = TKN_OP_TYPE_CMN;
    tokens[tokens_ind].token_op.op_cmn = tkn_cmn_name;
    tokens[tokens_ind].tkn_start = tkn_start;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_op_unr( Token *tokens, size_t tokens_ind, char *tkn_start, op_unr_t tkn_unr )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_OP;
    tokens[tokens_ind].token_op.tkn_op_type = TKN_OP_TYPE_UNR;
    tokens[tokens_ind].token_op.op_unr = tkn_unr;
    tokens[tokens_ind].tkn_start = tkn_start;
}

//! @note DO REALLOC_WRP BEFORE USING!
inline void token_put_op_bin( Token *tokens, size_t tokens_ind, char *tkn_start, op_bin_t tkn_bin )
{
    assert(tokens);
    tokens[tokens_ind].token_type = TKN_TYPE_OP;
    tokens[tokens_ind].token_op.tkn_op_type = TKN_OP_TYPE_BIN;
    tokens[tokens_ind].token_op.op_bin = tkn_bin;
    tokens[tokens_ind].tkn_start = tkn_start;
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

    char *prev_file_ptr = file.ptr;

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
                token_put_op_cmn( tokens, tokens_ind, file.ptr, TKN_OP_OPN_BRACKET );
            else
                token_put_op_cmn( tokens, tokens_ind, file.ptr, TKN_OP_CLS_BRACKET );
            tokens_ind++;
            file.ptr++;
        }
        else if ( *file.ptr == '\0')
        {
            REALLOC_ARR_WRP(tokens, Token);
            token_put_op_cmn( tokens, tokens_ind, file.ptr, TKN_OP_END );
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
                fprintf(stderr, "Error during parsing to tokens:\n"
                                "number expected, found: %s", file.ptr);
                return DIFF_STATUS_ERROR_TOKEN_PARSING;
            }

            REALLOC_ARR_WRP(tokens, Token);
            token_put_num( tokens, tokens_ind, file.ptr, num );
            file.ptr += read_chars;
            tokens_ind++;
        }
        else if ( (prev_file_ptr = file.ptr, op_unr = check_is_op_unr(&file) ) != 0 )
        {
            REALLOC_ARR_WRP(tokens, Token);
            token_put_op_unr( tokens, tokens_ind, prev_file_ptr, op_unr );
            tokens_ind++;
        }
        else if ( (prev_file_ptr = file.ptr, op_bin = check_is_op_bin(&file) ) != 0 )
        {
            REALLOC_ARR_WRP(tokens, Token);
            token_put_op_bin( tokens, tokens_ind, prev_file_ptr, op_bin );
            tokens_ind++;
        }
        else
        {
            prev_file_ptr = file.ptr;
            int is_new = 0;
            var_t var = get_var_id( &file, vars_names, vars_names_ind, &is_new );
            if (is_new)
            {
                vars_names_ind++;
                REALLOC_ARR_WRP(vars_names, char*);
            }

            REALLOC_ARR_WRP(tokens, Token);
            token_put_var( tokens, tokens_ind, prev_file_ptr, var );
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

//! @brief Searches for the first previous significant char in string
//! 'string', starting from 'ptr'. If no significant chars are found, 0 is returned.
inline char get_prev_significant_char( const char *string, const char *ptr )
{
    while ( ptr >= string && isspace(*ptr) )
        ptr--;

    if (ptr < string)
        return 0;
    return *ptr;
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
            char prev_c = get_prev_significant_char(file->str, file->ptr - 1);
            if ( (op_id == (op_unr_t) OP_MINUS || op_id == (op_unr_t) OP_PLUS)
                && !( prev_c == 0 || prev_c == '(' ) )
                return 0; // this is not unary minus/plus! this is a binary one!

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



inline void print_rec_fall_err_msg( Token tkn, const char *expected )
{
    fprintf(STREAM_ERROR, "Syntax error: expected <%s>, found:\n", expected);
    for (size_t ind = 0; ind < ERROR_MSG_LEN && tkn.tkn_start[ind] != '\0'; ind++)
    {
        putc( tkn.tkn_start[ind], STREAM_ERROR );
    }
    putc( '\n', STREAM_ERROR );
}

inline int is_op_cmn_and_check_name( Token tkn, TokenOpCmnName cmn_name )
{
    return tkn.token_type == TKN_TYPE_OP
        && tkn.token_op.tkn_op_type == TKN_OP_TYPE_CMN
        && tkn.token_op.op_cmn == cmn_name;
}

inline int is_op_unr_and_check_prior( Token tkn, TokenOpUnrPrior prior )
{
    return tkn.token_type == TKN_TYPE_OP
        && tkn.token_op.tkn_op_type == TKN_OP_TYPE_UNR
        && op_unr_list[tkn.token_op.op_unr].prior == prior;
}

inline int is_op_bin_and_check_prior( Token tkn, TokenOpBinPrior prior )
{
    return tkn.token_type == TKN_TYPE_OP
        && tkn.token_op.tkn_op_type == TKN_OP_TYPE_BIN
        && op_bin_list[tkn.token_op.op_bin].prior == prior;
}

#define curr_token tkns->tokens[tkns->curr_tkn_ind]

TreeNode *diff_get_num( TknsAndVars *tkns, Tree *tree )
{
    assert(tkns);
    assert(tree);

    if ( curr_token.token_type == TKN_TYPE_NUM )
    {
        TreeNode *node = diff_new_const_node( tree, curr_token.num );
        tkns->curr_tkn_ind++;
        return node;
    }
    else
    {
        return NULL;
    }
}

TreeNode *diff_get_var( TknsAndVars *tkns, Tree *tree )
{
    assert(tkns);
    assert(tree);

    if ( curr_token.token_type == TKN_TYPE_VAR )
    {
        TreeNode *node = diff_new_var_node( tree, curr_token.var );
        tkns->curr_tkn_ind++;
        return node;
    }
    else
    {
        return NULL;
    }
}

TreeNode *diff_get_primal( TknsAndVars *tkns, Tree *tree )
{
    assert(tkns);
    assert(tree);

    TreeNode *node = NULL;
    Token tmp_token = curr_token;
    if ( is_op_cmn_and_check_name( curr_token, TKN_OP_OPN_BRACKET ) )
    {
        tkns->curr_tkn_ind++;
        tmp_token = curr_token;
        node = diff_get_addive( tkns, tree );
        if (!node)
        {
            print_rec_fall_err_msg(tmp_token, "addive");
            return NULL;
        }

        if ( !is_op_cmn_and_check_name( curr_token, TKN_OP_CLS_BRACKET ) )
        {
            print_rec_fall_err_msg(curr_token, "closing bracket: ')'");
            return NULL;
        }

        tkns->curr_tkn_ind++;
        return node;
    }
    else if ( ( node = diff_get_num(tkns, tree) ) != NULL )
        return node;
    else if ( ( node = diff_get_var(tkns, tree) ) != NULL )
        return node;
    else
    {
        print_rec_fall_err_msg(tmp_token, "primal");
        return NULL;
    }

    assert(0);
    return NULL;
}

TreeNode *diff_get_unr( TknsAndVars *tkns, Tree *tree )
{
    assert(tkns);
    assert(tree);

    Token tmp_token;
    if ( is_op_unr_and_check_prior( curr_token, TKN_OP_UNR_DEFAULT ) )
    {
        TreeNode *node_op_unr = diff_new_op_unr_node( tree, curr_token.token_op.op_unr );
        tkns->curr_tkn_ind++;

        tmp_token = curr_token;
        TreeNode *node_primal = diff_get_primal( tkns, tree );
        if (!node_primal)
        {
            print_rec_fall_err_msg( tmp_token, "primal" );
            return NULL;
        }

        tree_hang_loose_node_at_left( tree, node_primal, node_op_unr );
        return node_op_unr;
    }
    else
    {
        tmp_token = curr_token;
        TreeNode *node_primal = diff_get_primal( tkns, tree );
        if (!node_primal)
        {
            print_rec_fall_err_msg( tmp_token, "primal" );
            return NULL;
        }

        return node_primal;
    }
}

TreeNode *diff_get_powive( TknsAndVars *tkns, Tree *tree )
{
    assert(tkns);
    assert(tree);

    Token tmp_token = curr_token;
    TreeNode *node_first = diff_get_unr( tkns, tree );
    if ( !node_first )
    {
        print_rec_fall_err_msg( tmp_token, "unr" );
        return NULL;
    }

    TreeNode *node_second = NULL;
    while ( is_op_bin_and_check_prior( curr_token, TKN_OP_BIN_GROUP_POW ) )
    {
        op_bin_t op = curr_token.token_op.op_bin;
        tkns->curr_tkn_ind++;

        node_second = diff_get_unr( tkns, tree );
        if ( !node_second )
        {
            print_rec_fall_err_msg( tmp_token, "unr" );
            return NULL;
        }

        TreeNode *node_new_op = diff_new_op_bin_node(tree, op);
        tree_hang_loose_node_at_left( tree, node_first, node_new_op );
        tree_hang_loose_node_at_right( tree, node_second, node_new_op );
        node_first = node_new_op;
    }

    return node_first;
}

TreeNode *diff_get_mulive( TknsAndVars *tkns, Tree *tree )
{
    assert(tkns);
    assert(tree);

    Token tmp_token = curr_token;
    TreeNode *node_first = diff_get_powive( tkns, tree );
    if ( !node_first )
    {
        print_rec_fall_err_msg( tmp_token, "powive" );
        return NULL;
    }

    TreeNode *node_second = NULL;
    while ( is_op_bin_and_check_prior( curr_token, TKN_OP_BIN_GROUP_MUL ) )
    {
        op_bin_t op = curr_token.token_op.op_bin;
        tkns->curr_tkn_ind++;

        node_second = diff_get_powive( tkns, tree );
        if ( !node_second )
        {
            print_rec_fall_err_msg( tmp_token, "powive" );
            return NULL;
        }

        TreeNode *node_new_op = diff_new_op_bin_node(tree, op);
        tree_hang_loose_node_at_left( tree, node_first, node_new_op );
        tree_hang_loose_node_at_right( tree, node_second, node_new_op );
        node_first = node_new_op;
    }

    return node_first;
}

TreeNode *diff_get_addive( TknsAndVars *tkns, Tree *tree)
{
    assert(tkns);
    assert(tree);

    Token tmp_token = curr_token;
    TreeNode *node_first = diff_get_mulive( tkns, tree );
    if ( !node_first )
    {
        print_rec_fall_err_msg( tmp_token, "mulive" );
        return NULL;
    }
    TreeNode *node_second  = NULL;
    while ( is_op_bin_and_check_prior( curr_token, TKN_OP_BIN_GROUP_ADD ) )
    {
        op_bin_t op = curr_token.token_op.op_bin;
        tkns->curr_tkn_ind++;

        node_second = diff_get_mulive( tkns, tree );
        if ( !node_second )
        {
            print_rec_fall_err_msg( tmp_token, "mulive" );
            return NULL;
        }

        TreeNode *node_new_op = diff_new_op_bin_node(tree, op);
        tree_hang_loose_node_at_left( tree, node_first, node_new_op );
        tree_hang_loose_node_at_right( tree, node_second, node_new_op );
        node_first = node_new_op;
    }

    return node_first;
}

DiffStatus diff_build_expr_tree_from_tokens( TknsAndVars *tkns, Tree *expr_tree )
{
    Token tmp_token = curr_token;
    TreeNode *root = diff_get_addive( tkns, expr_tree );
    if (!root)
    {
        print_rec_fall_err_msg( tmp_token, "addive" );
        return DIFF_STATUS_ERROR_REC_FALL;
    }

    tree_hang_loose_node_as_root(expr_tree, root);

    if ( !is_op_cmn_and_check_name(curr_token, TKN_OP_END) )
    {
        print_rec_fall_err_msg(curr_token, "end: '\\0'");
        return DIFF_STATUS_ERROR_REC_FALL;
    }

    return DIFF_STATUS_OK;
}

DiffStatus diff_assemble_expr_tree( TknsAndVars *tkns, Tree *expr_tree )
{
    assert(tkns);
    assert(tkns->tokens);
    assert(expr_tree);

    tree_ctor(expr_tree, sizeof(ExprNodeData), NULL, expr_node_data_print);

    DiffStatus err = diff_build_expr_tree_from_tokens( tkns, expr_tree );

    if (err)
    {
        tree_dtor(expr_tree);
        return err;
    }

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
