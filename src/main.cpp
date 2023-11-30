#include <stdio.h>

#include "differentiator.h"
#include "differentiator_input.h"
#include "differentiator_output.h"

// TODO - детерминированный конечный автомат

int main()
{
    FileBuf file_buf = read_file_to_buf( "test.txt" );

    preprocess_detach_not_alnums( &file_buf );

    // debug
    printf("<%s>\n", file_buf.buf);
    // deibg end

    ParsedFileBuf parsed_file = {};
    DiffStatus err = parse_file_buf(file_buf, &parsed_file);
    if (err)
    {
        buf_free(&file_buf);
        parsed_file_buf_dtor(&parsed_file);
        diff_print_status_message(stderr, err);
        return err;
    }

    buf_free(&file_buf); // ??? because all tokens are now stored in parsed_buf.tokens_buf??

    VarsOpsRaw vars_ops_raw = { NULL, 0, NULL, 0, NULL, 0 };
    err = diff_assemble_vars_ops_raw( parsed_file, &vars_ops_raw );
    if (err)
    {
        parsed_file_buf_dtor(&parsed_file);
        vars_ops_raw_dtor(&vars_ops_raw);

        diff_print_status_message(stderr, err);
        return err;
    }

    // debug
    size_t vars_xxx     = 0;
    size_t ops_unr_xxx  = 0;
    size_t ops_bin_xxx  = 0;
    for (size_t ind = 0; ind < parsed_file.n_tokens; ind++)
    {
        printf( "%llu : <%s>", ind, parsed_file.tokens[ind] );
        if ( vars_ops_raw.vars_for_parsing[vars_xxx].token_ind == ind )
        {
            printf(" (var_id : <%d>) ", (int) vars_ops_raw.vars_for_parsing[vars_xxx].var_id );
            vars_xxx++;
        }
        if ( vars_ops_raw.ops_unr_for_parsing[ops_unr_xxx].token_ind == ind )
        {
            printf(" (op_unr_id : <%d>) ", (int) vars_ops_raw.ops_unr_for_parsing[ops_unr_xxx].op_unr_id );
            ops_unr_xxx++;
        }
        if ( vars_ops_raw.ops_bin_for_parsing[ops_bin_xxx].token_ind == ind )
        {
            printf(" (op_bin_id : <%d>) ", (int) vars_ops_raw.ops_bin_for_parsing[ops_bin_xxx].op_bin_id );
            ops_bin_xxx++;
        }
        printf("\n");
    }

    printf("Variables, ids and names:\n");
    for (size_t ind = 0; ind < vars_ops_raw.n_var_names; ind++)
    {
        printf("%llu : <%s>\n", ind, vars_ops_raw.vars_names[ind]);
    }
    // debug end

    Tree expr_tree = {};
    err = diff_assemble_expr_tree(parsed_file, &vars_ops_raw, &expr_tree);
    if (err)
    {
        parsed_file_buf_dtor(&parsed_file);
        vars_ops_raw_dtor(&vars_ops_raw);
        tree_dtor(&expr_tree);

        diff_print_status_message(stderr, err);
        return err;
    }

    Expression expr = diff_assemble_expression( &expr_tree, vars_ops_raw.vars_names, vars_ops_raw.n_var_names );

    parsed_file_buf_dtor(&parsed_file);
    vars_ops_raw_dtor(&vars_ops_raw);

    // end of input

    // ...some work with expression...


    diff_expr_dtor(&expr);

    return 0;
}
