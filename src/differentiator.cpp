#include "differentiator.h"

#include <assert.h>

void diff_expr_dtor( Expression *expr_ptr )
{
    assert(expr_ptr);

    tree_dtor(&expr_ptr->expr_tree);

    for (size_t ind = 0; ind < expr_ptr->vars_names_len; ind++)
    {
        assert(expr_ptr->vars_names[ind]);
        free(expr_ptr->vars_names[ind]);
    }
    free(expr_ptr->vars_names);
}
