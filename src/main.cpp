#include <stdio.h>

#include "differentiator.h"
#include "differentiator_input.h"
#include "differentiator_output.h"
#include "differentiator_dump.h"
#include "utils.h"

#include "..\..\..\mylibheaders\onegin.h"

inline int get_vars_values_from_user( const Expression *expr_ptr, double *vars_values )
{
    printf( "Do you want to evaluate the expression? [y/n]\n" );
    if ( getchar() == 'y' )
    {
        for (var_t var = 0; var < expr_ptr->vars_names_len; var++)
        {
            do
            {
                clear_inp(stdin);
                printf("%s: ", expr_ptr->vars_names[var]);
            } while (scanf("%lf", &vars_values[var]) != 1);
        }
    }
    else
    {
        clear_inp(stdin);
        printf( "As far as I've understood, you don't want to evaluate the expression.\n" );
        return 0;
    }

    clear_inp(stdin);

    return 1;
}

int main()
{
    // ДАННЫЙ main() СЛУЖИТ ДЕМОНСТРАЦИОННЫМ ПРИМЕРОМ ВОЗМОЖНОСТЕЙ ДИФФЕРЕНЦИАТОРА

    FileBuf file_buf = read_file_to_buf( "test.txt" );

    TknsAndVars parsed_tokens = {};
    DiffStatus err = parse_file_buf({file_buf.buf, file_buf.buf_size, file_buf.buf}, &parsed_tokens);
    if (err)
    {
        buf_free(&file_buf);
        parsed_tokens_and_vars_dtor(&parsed_tokens);
        diff_print_status_message(stderr, err);
        return err;
    }

    Tree expr_tree = {};
    err = diff_assemble_expr_tree(&parsed_tokens, &expr_tree);
    if (err)
    {
        buf_free(&file_buf);
        parsed_tokens_and_vars_dtor(&parsed_tokens);
        tree_dtor(&expr_tree);

        diff_print_status_message(stderr, err);
        return err;
    }

    Expression expr = diff_assemble_expression( &expr_tree, parsed_tokens.vars_names, parsed_tokens.n_vars );

    buf_free(&file_buf);
    parsed_tokens_and_vars_dtor(&parsed_tokens);
    // end of input

    // ...some work with expression...
    diff_dump( &expr );
    diff_start_latex_doc( );


    diff_write_latex_section( LATEX_SECTION_GIVEN );
    diff_write_latex_expr( &expr );


    diff_simplify(&expr);
    diff_write_latex_section( LATEX_SECTION_SIMPLIFY );
    diff_write_latex_rand_phrase();
    diff_write_latex_expr(&expr);


    double *vars_values = (double*) calloc( expr.vars_names_len, sizeof(double) );
    if ( get_vars_values_from_user( &expr, vars_values ) )
    {
        double x = diff_evaluate( &expr, vars_values );
        fprintf(stdout, "Result: <%g>\n", x);
    }


    Expression diffed_expr = diff_diff(&expr, 0);


    diff_dump(&diffed_expr);
    diff_write_latex_section( LATEX_SECTION_DIFF );
    diff_write_latex_rand_phrase();
    diff_write_latex_expr( &diffed_expr );


    diff_simplify(&diffed_expr);
    diff_write_latex_section( LATEX_SECTION_SIMPLIFY );
    diff_write_latex_rand_phrase();
    diff_write_latex_expr(&diffed_expr);
    diff_dump(&diffed_expr);

    int is_diffed_expr_const = !is_subtree_var( tree_get_root( &diffed_expr.expr_tree ), 0);
    printf( "Is diffed expression a const (concerning var 0): %s.\n",
            ( is_diffed_expr_const ? "yes" : "no" ) );


    if ( get_vars_values_from_user( &expr, vars_values ) )
    {
        double x = diff_evaluate( &diffed_expr, vars_values );
        fprintf(stdout, "Result in diffed expression: <%g>\n", x);
    }


    diff_print_expr_as_text( stdout, &diffed_expr );


    diff_end_latex_doc();


    diff_expr_dtor(&expr);
    diff_expr_dtor(&diffed_expr);
    FREE(vars_values);

    printf("all is good...\n");

    return 0;
}

