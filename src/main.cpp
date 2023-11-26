#include <stdio.h>

#include "differentiator.h"
#include "differentiator_input.h"

int main()
{
    FileBuf file_buf = read_file_to_buf( "test.txt" );

    preprocess_detach_not_alnums( &file_buf );

    // printf("<%s>\n", file_buf.buf);

    ParsedFileBuf parsed_file = {};
    DiffStatus status = parse_file_buf(file_buf, &parsed_file);
    if (!status)
    {
        FREE(file_buf.buf);
        FREE(parsed_file.tokens);
        diff_print_status_message(stderr, status);
        return status;
    }

    // for (size_t ind = 0; ind < parsed_file.n_tokens; ind++)
    // {
    //     printf( "%llu : <%s>\n", ind, parsed_file.tokens[ind] );
    // }

    VarsOpsRaw vars_ops_raw = {};
    status = diff_get_vars_ops_raw( parsed_file, &vars_ops_raw );
    if (!status)
    {
        FREE(file_buf.buf);
        FREE(parsed_file.tokens);

        diff_print_status_message(stderr, status);
        return status;
    }

    return 0;
}
