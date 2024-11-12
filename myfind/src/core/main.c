#define _DEFAULT_SOURCE 2.19

#include "../lib/lib.h"

void print_ast(struct token *ast)
{
    if (ast == NULL)
    {
        return;
    }
    printf("ast: %u value %s\n", ast->type, ast->value);
    printf("left:\n");
    print_ast(ast->left);
    printf("right:\n");
    print_ast(ast->right);
    return;
}

int main(int argc, char **argv)
{
    int cr = 0;
    char **entry_point_list = calloc(argc, sizeof(char *));
    char **args_list = calloc(argc, sizeof(char *));
    parse_arg(argv, entry_point_list, args_list);

    int i = 0;

    struct queue *q = lexing(args_list);
    struct token *ast = build_ast(q);
    if (!(entry_point_list[0]))
    {
        entry_point_list[0] = no_entry_point();
    }
    while (entry_point_list[i])
    {
        struct manage_entry_point *entry =
            parse_entry_point(entry_point_list[i], &cr);
        eval_ast(ast, entry);
        entry_point_destroy(entry);
        i++;
    }
    free_ast(ast);
    free_list(entry_point_list);
    free_list(args_list);
    return cr;
}
