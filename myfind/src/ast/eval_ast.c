#include "../lib/lib.h"

int eval_ast(struct token *ast, struct manage_entry_point *entry_point)
{
    if (ast == NULL)
    {
        return 0;
    }
    if (ast->type == EXPR_AND)
    {
        return eval_ast(ast->left, entry_point)
            && eval_ast(ast->right, entry_point);
    }
    else if (ast->type == EXPR_OR)
    {
        return eval_ast(ast->left, entry_point)
            || eval_ast(ast->right, entry_point);
    }
    else if (ast->type == EXPR_NOT)
    {
        return !eval_ast(ast->left, entry_point);
    }
    else
    {
        return get_function(ast->type)(entry_point, ast->value);
    }
    return 0;
}
