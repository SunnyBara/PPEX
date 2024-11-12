#include "ast.h"

void free_ast(struct token *ast)
{
    if (ast == NULL)
    {
        return;
    }
    free_ast(ast->left);
    free_ast(ast->right);
    token_destroy(ast);
    return;
}
