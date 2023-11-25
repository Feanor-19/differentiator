#include "differentiator_common.h"

ExprNodeData diff_get_data( const Expression *expr_ptr, const TreeNode *node_ptr )
{
    ExprNodeData expr_node_data = {};
    TreeStatus status = tree_get_data( &expr_ptr->expr_tree, node_ptr, &expr_node_data );
    if (!status)
        return {ERROR};
    return expr_node_data;
}

void diff_print_status_message( FILE *stream, DiffStatus status )
{
    fprintf(stream, "%s", diff_status_messages[status]);
}
