#include "differentiator_output.h"

#include "tex_phrases.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>


inline int are_parentheses_needed( op_prior_t prior,
                                   op_prior_t prev_op_prior,
                                   const TreeNode *node_ptr )
{
    TreeNode *parent = tree_get_parent( node_ptr );
    return prior < prev_op_prior
        || (parent
            && diff_get_type( parent ) == OP_BIN
            && diff_get_op_bin( parent ) == OP_POW
            && tree_get_left_child( parent ) == node_ptr )
        || ( parent
            && diff_get_type( parent ) == OP_BIN
            && diff_get_op_bin( parent ) == OP_DIV
            && tree_get_right_child( parent ) == node_ptr );
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
        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
            fprintf(stream, "(");
        fprintf(stream, "%s(", op_unr_list[diff_get_op_unr(node_ptr)].name);
        print_as_text_expr_tree_node(stream, expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(stream, ")");
        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
            fprintf(stream, ")");
        break;
    case OP_BIN:
        prior = op_bin_list[diff_get_op_bin(node_ptr)].prior;
        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
            fprintf(stream, "(");

        print_as_text_expr_tree_node(stream, expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(stream, " %s ", op_bin_list[diff_get_op_bin(node_ptr)].name);
        print_as_text_expr_tree_node(stream, expr_ptr, tree_get_right_child(node_ptr), prior);

        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
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


static FILE *TEX_STREAM    = NULL;
static char *FILE_NAME     = NULL;



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
        fprintf(TEX_STREAM, " %g ", diff_get_const(node_ptr));
        break;
    case VAR:
        fprintf(TEX_STREAM, " %s ", expr_ptr->vars_names[ diff_get_var(node_ptr) ]);
        break;
    case OP_UNR:
        prior = op_unr_list[diff_get_op_unr(node_ptr)].prior;
        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
            fprintf(TEX_STREAM, "(");
        if ( diff_get_op_unr(node_ptr) != OP_MINUS && diff_get_op_unr(node_ptr) != OP_PLUS )
            fprintf( TEX_STREAM, "\\" );

        fprintf(TEX_STREAM, "%s{", op_unr_list[diff_get_op_unr(node_ptr)].name);
        print_as_latex_expr_tree_node(expr_ptr, tree_get_left_child(node_ptr), prior);
        fprintf(TEX_STREAM, "}");
        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
            fprintf(TEX_STREAM, ")");
        break;
    case OP_BIN:
        prior = op_bin_list[diff_get_op_bin(node_ptr)].prior;
        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
            fprintf(TEX_STREAM, "(");

        if ( diff_get_op_bin( node_ptr ) == OP_DIV )
        {
            fprintf(TEX_STREAM, "\\frac{");
            print_as_latex_expr_tree_node(expr_ptr, tree_get_left_child(node_ptr), prior);
            fprintf(TEX_STREAM, " }{ ");
            print_as_latex_expr_tree_node(expr_ptr, tree_get_right_child(node_ptr), prior);
            fprintf(TEX_STREAM, " } ");
        }
        else
        {
            print_as_latex_expr_tree_node(expr_ptr, tree_get_left_child(node_ptr), prior);
            fprintf(TEX_STREAM, " %s ", op_bin_list[diff_get_op_bin(node_ptr)].name);
            print_as_latex_expr_tree_node(expr_ptr, tree_get_right_child(node_ptr), prior);
        }

        if ( are_parentheses_needed(prior, prev_op_prior, node_ptr) )
            fprintf(TEX_STREAM, ")");
        break;
    case ERROR:
        fprintf(TEX_STREAM, " ERROR_NODE ");
        break;
    default:
        assert(0);
    }
    return DIFF_STATUS_OK;
}

inline DiffStatus write_latex_header( )
{
    fprintf( TEX_STREAM,
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

DiffStatus diff_start_latex_doc(  )
{
    if ( TEX_STREAM )
        return DIFF_STATUS_ERROR_PREV_TEX_NOT_ENDED_YET;

    mkdir( LATEX_DIR_NAME );

    DIR *dir = opendir( LATEX_DIR_NAME );
    if (!dir)
        return DIFF_STATUS_ERROR_CANT_CREATE_TEX_FOLDER;

    closedir(dir);

    TEX_STREAM = fopen( LATEX_DIR_NAME LATEX_FILE_NAME, "w" );
    if (!TEX_STREAM)
        return DIFF_STATUS_ERROR_CANT_CREATE_TEX_FILE;

    write_latex_header();

    srand( (unsigned int) time(NULL) );

    return DIFF_STATUS_OK;
}

DiffStatus diff_write_latex_expr( const Expression *expr_ptr )
{
    if (!TEX_STREAM)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf( TEX_STREAM, "$$ ");
    print_as_latex_expr_tree_node( expr_ptr, tree_get_root( &expr_ptr->expr_tree ), 0 );
    fprintf( TEX_STREAM, " $$\n\n");

    return DIFF_STATUS_OK;
}

DiffStatus diff_write_latex_rand_phrase( )
{
    if (!TEX_STREAM)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf( TEX_STREAM, "%s\n\n", TEX_PHRASES[rand() % TEX_PHRASES_NUM] );

    return DIFF_STATUS_OK;
}

DiffStatus diff_write_latex_section( const char *section_name )
{
    if (!TEX_STREAM)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf(TEX_STREAM, "\\section{%s}\n\n", section_name);

    return DIFF_STATUS_OK;
}

DiffStatus diff_end_latex_doc()
{

    if (!TEX_STREAM)
        return DIFF_STATUS_ERROR_NO_TEX_FILE_STARTED;

    fprintf(TEX_STREAM, "\\end{document}\n");

    fclose(TEX_STREAM);
    TEX_STREAM = NULL;

    // const char cmd[] = "cd " LATEX_DIR_NAME "; pdflatex .\\" LATEX_FILE_NAME "; cd .\\..\\" ;
    const char cmd[] = "cd .\\LaTeX && pdflatex latex.tex && cd .." ;

    system( cmd );

    FREE(FILE_NAME);


    return DIFF_STATUS_OK;
}
