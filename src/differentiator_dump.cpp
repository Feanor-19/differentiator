#include "differentiator_dump.h"

#include <assert.h>

inline FILE *create_and_open_dot_file()
{
	return fopen(DUMP_DOT_NAME, "w");
}

inline void run_dot_to_create_database_img()
{
	system("dot " DUMP_DOT_NAME " -Tjpg -o " DUMP_IMG_NAME);
}

inline void write_dot_file( FILE *dot_file, Expression *expr )
{
	assert(dot_file);
	assert(expr);

#define COLOR_BG                "#2D4059"
#define COLOR_NODE_COLOR        "#ECC237"
#define COLOR_CONST_NODE_FILL   "#EA5455"
#define COLOR_VAR_NODE_FILL     "#8ccb5e"
#define COLOR_OP_NODE_FILL      "#FFD460"
#define COLOR_LABEL_COLOR       "#EA5455"
#define COLOR_LABEL_FILL        "#ECC237"
#define COLOR_EDGE              "#F07B3F"
// #define COLOR_EDGE_RIGHT    "#FFD460"
// #define COLOR_EDGE          "#8ccb5e"


    // Dot header
    fprintf(dot_file,   "digraph{\n"
                        "splines=ortho;\n"
                        "bgcolor=\"" COLOR_BG "\";"
                        "\n\n\n");


    // Nodes with data
    TreeNode *curr_node = expr->expr_tree.head_of_all_nodes;
    TreeNode **nodes_arr = (TreeNode**) calloc( expr->expr_tree.nodes_count, sizeof(TreeNode) );
    for (size_t ind = 0; ind < expr->expr_tree.nodes_count; ind++)
    {
        if ( curr_node == NULL )
        {
            fprintf(stderr, "ERROR: something wrong with listing of nodes!\n");
            break;
        }

        ExprNodeData node_data = diff_get_data(curr_node);
        if ( node_data.type == CONST )
        {
            fprintf(dot_file,   "NODE_%llu[shape=\"record\", fontname=\"verdana\",\n"
                                "style=bold, style=filled,\ncolor=\"" COLOR_NODE_COLOR "\""
                                ", fillcolor=\"" COLOR_CONST_NODE_FILL "\",\n"
                                "label = %g];\n\n",
                                ind, node_data.cnst);
        }
        else if ( node_data.type == VAR )
        {
            fprintf(dot_file,   "NODE_%llu[shape=\"record\", fontname=\"verdana\",\n"
                                "style=bold, style=filled,\ncolor=\"" COLOR_NODE_COLOR "\""
                                ", fillcolor=\"" COLOR_VAR_NODE_FILL "\",\n"
                                "label = %s];\n\n",
                                ind, expr->vars_names[node_data.var]);
        }
        else if ( node_data.type == OP_BIN )
        {
            fprintf(dot_file,   "NODE_%llu[shape=\"record\", fontname=\"verdana\",\n"
                                "style=bold, style=filled,\ncolor=\"" COLOR_NODE_COLOR "\""
                                ", fillcolor=\"" COLOR_OP_NODE_FILL "\",\n"
                                "label = \"%s\"];\n\n",
                                ind, op_bin_list[node_data.op_bin].name);
        }
        else if ( node_data.type == OP_UNR )
        {
            fprintf(dot_file,   "NODE_%llu[shape=\"record\", fontname=\"verdana\",\n"
                                "style=bold, style=filled,\ncolor=\"" COLOR_NODE_COLOR "\""
                                ", fillcolor=\"" COLOR_OP_NODE_FILL "\",\n"
                                "label = \"%s\"];\n\n",
                                ind, op_unr_list[node_data.op_unr].name);
        }

        nodes_arr[ind] = curr_node;
        curr_node = curr_node->next;
    }


    // Edges
    for (size_t ind = 0; ind < expr->expr_tree.nodes_count; ind++)
    {
        size_t left = 0;
        size_t right = 0;
        for (size_t i = 0; i < expr->expr_tree.nodes_count; i++)
        {
            if ( nodes_arr[i] == nodes_arr[ind]->left )
            {
                left = i;
            }
            else if ( nodes_arr[i] == nodes_arr[ind]->right )
            {
                right = i;
            }
        }

        if ( nodes_arr[ind]->left )
        {
            fprintf(dot_file, "NODE_%llu->NODE_%llu"
							  "[color=\"" COLOR_EDGE "\", "
							  "penwidth=2];\n",
                              ind, left);
        }

        if ( nodes_arr[ind]->right )
        {
            fprintf(dot_file, "NODE_%llu->NODE_%llu[color=\"" COLOR_EDGE "\", "
							  "penwidth=2];\n", ind, right);
        }

        if ( nodes_arr[ind]->left && nodes_arr[ind]->right )
        {
            fprintf(dot_file,   "NODE_%llu->NODE_%llu[style=invis];\n"
                                "{rank=same NODE_%llu NODE_%llu}",
                                left, right, left, right);
        }
    }


    fprintf(dot_file, "\n}\n");

#undef COLOR_BG
#undef COLOR_OCCUP_NODE_COLOR
#undef COLOR_OCCUP_NODE_FILL
#undef COLOR_FREE_NODE_COLOR
#undef COLOR_FREE_NODE_FILL
#undef COLOR_LABEL_COLOR
#undef COLOR_LABEL_FILL
#undef COLOR_EDGE_PREV
#undef COLOR_EDGE_NEXT
#undef COLOR_EDGE_FREE

	free(nodes_arr);
}

inline void show_dump_img()
{
	system(DUMP_IMG_NAME);
}

void diff_dump(Expression *expr)
{
    assert(expr);

    FILE *file = create_and_open_dot_file();

    write_dot_file(file, expr);

    fclose( file );

    run_dot_to_create_database_img();

    show_dump_img();
}
