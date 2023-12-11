#include "differentiator_output.h"

#include "tex_phrases.h"

#include <assert.h>
#include <stdlib.h>


// TODO - обосновано ли?
FILE *tex_stream = NULL;



inline int are_parentheses_needed( op_prior_t prior, op_prior_t prev_op_prior )
{
    return prior <= prev_op_prior;
}

static DiffStatus print_as_text_expr_tree_node( FILE *stream,
                                                const Expression *expr_ptr,
                                                const TreeNode *node_ptr,
                                                op_prior_t prev_op_prior)
{
    assert(expr_ptr);
    assert(node_ptr);

    op_prior_t prior = 0;
    switch (diff_get_type(node_ptr))
    {
    case CONST:
        fprintf(stream, "%g", diff_get_const(node_ptr));
        break;
    case VAR:
        fprintf(stream, "%s", expr_ptr->vars_names[ diff_get_var(node_ptr) ]);
        break;
    case OP_UNR:
        prior = op_unr_list[diff_get_op_unr(node_ptr)].prior;
        fprintf(stream, "%s(", op_unr_list[diff_get_op_unr(node_ptr)].name);
        print_as_text_expr_tree_node(stream, expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(stream, ")");
        break;
    case OP_BIN:
        prior = op_bin_list[diff_get_op_bin(node_ptr)].prior;
        if ( are_parentheses_needed(prior, prev_op_prior) )
            fprintf(stream, "(");

        print_as_text_expr_tree_node(stream, expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(stream, " %s ", op_bin_list[diff_get_op_bin(node_ptr)].name);
        print_as_text_expr_tree_node(stream, expr_ptr, tree_get_right_child(node_ptr), prior);

        if ( are_parentheses_needed(prior, prev_op_prior) )
            fprintf(stream, ")");
        break;
    case ERROR:
        fprintf(stream, "ERROR_NODE");
        break;
    default:
        assert(0);
    }
    return DIFF_STATUS_OK;
}

DiffStatus diff_print_expr_as_text( FILE *stream, const Expression *expr_ptr )
{
    assert(stream);
    assert(expr_ptr);

    DiffStatus err = print_as_text_expr_tree_node( stream,
                                                   expr_ptr,
                                                   tree_get_root( &expr_ptr->expr_tree ),
                                                   0 );
    return err;
}

static DiffStatus print_as_latex_expr_tree_node( const Expression *expr_ptr,
                                                 const TreeNode *node_ptr,
                                                 op_prior_t prev_op_prior )
{
    assert(expr_ptr);
    assert(node_ptr);

    op_prior_t prior = 0;
    switch (diff_get_type(node_ptr))
    {
    case CONST:
        fprintf(tex_stream, " %g ", diff_get_const(node_ptr));
        break;
    case VAR:
        fprintf(tex_stream, " %s ", expr_ptr->vars_names[ diff_get_var(node_ptr) ]);
        break;
    case OP_UNR:
        prior = op_unr_list[diff_get_op_unr(node_ptr)].prior;
        fprintf(tex_stream, "%s{", op_unr_list[diff_get_op_unr(node_ptr)].name);
        print_as_latex_expr_tree_node(expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(tex_stream, "}");
        break;
    case OP_BIN:
        prior = op_bin_list[diff_get_op_bin(node_ptr)].prior;
        if ( prior < prev_op_prior )
            fprintf(tex_stream, "(");

        if ( diff_get_op_bin( node_ptr ) == OP_DIV )
        {
            fprintf(tex_stream, "\\frac{");
            print_as_latex_expr_tree_node(expr_ptr, tree_get_left_child(node_ptr), prior);
            fprintf(tex_stream, " }{ ");
            print_as_latex_expr_tree_node(expr_ptr, tree_get_right_child(node_ptr), prior);
            fprintf(tex_stream, " } ");
        }
        else
        {
            print_as_latex_expr_tree_node(expr_ptr, tree_get_left_child(node_ptr), prior);
            fprintf(tex_stream, " %s ", op_bin_list[diff_get_op_bin(node_ptr)].name);
            print_as_latex_expr_tree_node(expr_ptr, tree_get_right_child(node_ptr), prior);
        }

        if ( are_parentheses_needed(prior, prev_op_prior) )
            fprintf(tex_stream, ")");
        break;
    case ERROR:
        fprintf(tex_stream, " ERROR_NODE ");
        break;
    default:
        assert(0);
    }
    return DIFF_STATUS_OK;
}

inline FILE *open_tex_file( const char *dir_name )
{
    const char* TEX_FILE_NAME = "raw.tex";

    char *file_name = (char*) calloc( strlen(dir_name) + strlen(TEX_FILE_NAME), sizeof(char) );

    FILE *ret = fopen( file_name, "w" );

    free(file_name);
    return ret;
}

inline DiffStatus write_latex_header( )
{
    fprintf( tex_stream,
        "\\documentclass[12pt]{article}\n"
        "\\usepackage[T2A]{fontenc}\n"
        "\\usepackage{mathtools}\n"
        "\\usepackage[utf8]{inputenc}\n"
        "\\usepackage[english, russian]{babel}\n"
        "\\usepackage{fancyhdr}\n"
        "\\usepackage{graphicx}\n"
        "\\usepackage{float}\n"
        "\\usepackage{booktabs}\n"
        "\\usepackage{amsmath}\n"
        "\\usepackage{amssymb}\n"
        "\\usepackage{indentfirst}\n\n"
        "\\title{Наглядное руководство по дифференцированию математических выражений}\n"
        "\\author{Матан Матанович}\n\n\n"
        "\\begin{document}\n"
        "\\maketitle\n"
        "\\newpage\n\n\n");

    return DIFF_STATUS_OK;
}

DiffStatus diff_start_latex_doc( const char *dir_name )
{
    assert(dir_name);

    if ( tex_stream )
        return DIFF_STATUS_ERROR_PREV_TEX_NOT_ENDED_YET;

    if ( !mkdir( dir_name ) )
        return DIFF_STATUS_ERROR_CANT_CREATE_TEX_FOLDER;

    tex_stream = open_tex_file( dir_name );
    if (!tex_stream)
        return DIFF_STATUS_ERROR_CANT_CREATE_TEX_FILE;

    write_latex_header();

    return DIFF_STATUS_OK;
}

DiffStatus diff_write_latex_expr( const Expression *expr_ptr )
{
    if (!tex_stream)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf( tex_stream, "$$ ");
    print_as_latex_expr_tree_node( expr_ptr, tree_get_root( &expr_ptr->expr_tree ), 0 );
    fprintf( tex_stream, " $$\n\n");

    return DIFF_STATUS_OK;
}

DiffStatus diff_write_latex_rand_phrase( )
{
    if (!tex_stream)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf( tex_stream, "%s\n\n", TEX_PHRASES[rand() % TEX_PHRASES_NUM] );

    return DIFF_STATUS_OK;
}

DiffStatus diff_write_latex_section( const char *section_name )
{
    if (!tex_stream)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf(tex_stream, "\\section{%s}\n\n", section_name);

    return DIFF_STATUS_OK;
}

DiffStatus diff_end_latex_doc()
{
    if (!tex_stream)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf(tex_stream, "\end{document}\n");

    fclose(tex_stream);
    tex_stream = NULL;

    // TODO - формирование пдф

    return DIFF_STATUS_OK;
}
