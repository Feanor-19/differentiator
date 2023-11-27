#include <stdio.h>

#include "differentiator.h"
#include "differentiator_input.h"

int main()
{
    FileBuf file_buf = read_file_to_buf( "test.txt" );

    preprocess_detach_not_alnums( &file_buf );

    // debug
    printf("<%s>\n", file_buf.buf);

    ParsedFileBuf parsed_file = {};
    DiffStatus err = parse_file_buf(file_buf, &parsed_file);
    if (err)
    {
        FREE(file_buf.buf);
        FREE(parsed_file.tokens);
        diff_print_status_message(stderr, err);
        return err;
    }

    VarsOpsRaw vars_ops_raw = { NULL, 0, NULL, 0, NULL, 0 };
    err = diff_get_vars_ops_raw( parsed_file, &vars_ops_raw );
    if (err)
    {
        FREE(file_buf.buf);
        FREE(parsed_file.tokens);
        FREE(vars_ops_raw.ops_bin_for_parsing);
        FREE(vars_ops_raw.ops_unr_for_parsing);
        FREE(vars_ops_raw.vars_for_parsing);

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

    return 0;
}
