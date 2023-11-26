#include <stdio.h>

#include "differentiator.h"

int main()
{
    FileBuf file_buf = read_file_to_buf( "test.txt" );

    preprocess_detach_not_alnums( &file_buf );

    // printf("<%s>\n", file_buf.buf);

    ParsedFileBuf parsed_file = parse_file_buf(file_buf);

    // for (size_t ind = 0; ind < parsed_file.n_tokens; ind++)
    // {
    //     printf( "%llu : <%s>\n", ind, parsed_file.tokens[ind] );
    // }

    return 0;
}
