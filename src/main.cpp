#include <stdio.h>

#include "differentiator.h"
#include "differentiator_input.h"
#include "differentiator_output.h"
#include "differentiator_dump.h"


int main()
{
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
    // diff_dump( &expr );
    printf("Entered expression: ");
    diff_print_expr(stdout, &expr);
    printf("\n");

    double arr[] = {2, 4, 0};
    double x = diff_evaluate( &expr, arr );
    fprintf(stdout, "Result: <%g>\n", x);

    Expression diffed_expr = diff_diff(&expr, 0);

    diff_dump(&diffed_expr);
    printf("Diffed expression: ");
    diff_print_expr(stdout, &diffed_expr);
    printf("\n");

    int is_diffed_expr_const = !is_subtree_var( tree_get_root( &diffed_expr.expr_tree ), 0);
    printf( "Is diffed expression a const (concerning var 0): %s.\n",
            ( is_diffed_expr_const ? "yes" : "no" ) );

    x = diff_evaluate( &diffed_expr, arr );
    fprintf(stdout, "Result in diffed expression: <%g>\n", x);

    diff_expr_dtor(&expr);
    diff_expr_dtor(&diffed_expr);

    printf("all is good...\n");

    return 0;
}

