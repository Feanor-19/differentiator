#include "differentiator.h"

#include <assert.h>

void expr_dtor( Expression *expr_ptr )
{
    assert(expr_ptr);

    tree_dtor(&expr_ptr->expr_tree);

    for (size_t ind = 0; ind < expr_ptr->vars_len; ind++)
    {
        assert(expr_ptr->vars[ind]);
        free(expr_ptr->vars[ind]);
    }


}
