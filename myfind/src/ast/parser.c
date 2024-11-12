#include "../lib/lib.h"

enum token_type flag_to_enum(char *flag)
{
    int i = 0;
    struct token_model parser[] = {
        { "-o", EXPR_OR },         { "-a", EXPR_AND },
        { "!", EXPR_NOT },         { ")", EXPR_PARENTHESIS },
        { "(", EXPR_PARENTHESIS }, { "-name", EXPR_NAME },
        { "-type", EXPR_TYPE },    { "-newer", EXPR_NEWER },
        { "-print", EXPR_PRINT },  { "-delete", EXPR_DELETE },
        { "-exec", EXPR_EXEC },    { "-execdir", EXPR_EXECDIR },
        { "-perm", EXPR_PERM },    { "-user", EXPR_USER },
        { "-group", EXPR_GROUP }
    };
    while (i < FLAG_NUMBERS)
    {
        if (strcmp(flag, parser[i].str) == 0)
        {
            return parser[i].type;
        }
        i++;
    }
    return parser[0].type;
}

int is_flag(char *flag)
{
    struct token_model parser[] = {
        { "-o", EXPR_OR },         { "-a", EXPR_AND },
        { "!", EXPR_NOT },         { ")", EXPR_PARENTHESIS },
        { "(", EXPR_PARENTHESIS }, { "-name", EXPR_NAME },
        { "-type", EXPR_TYPE },    { "-newer", EXPR_NEWER },
        { "-print", EXPR_PRINT },  { "-delete", EXPR_DELETE },
        { "-exec", EXPR_EXEC },    { "-execdir", EXPR_EXECDIR },
        { "-perm", EXPR_PERM },    { "-user", EXPR_USER },
        { "-group", EXPR_GROUP }
    };
    int i = 0;
    while (i < FLAG_NUMBERS)
    {
        if (strcmp(flag, parser[i].str) == 0)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

int is_operator(enum token_type type)
{
    switch (type)
    {
    case EXPR_OR:
    case EXPR_AND:
    case EXPR_NOT:
        return 1;
    default:
        return 0;
    }
}
int is_test(enum token_type type)
{
    switch (type)
    {
    case EXPR_NAME:
    case EXPR_TYPE:
    case EXPR_NEWER:
        return 1;
    default:
        return 0;
    }
}

int is_action(enum token_type type)
{
    switch (type)
    {
    case EXPR_PRINT:
    case EXPR_DELETE:
    case EXPR_EXEC:
    case EXPR_EXECDIR:
        return 1;
    default:
        return 0;
    }
}

int is_opener(struct token *token)
{
    if (token->type == EXPR_PARENTHESIS && strcmp(token->value, "(") == 0)
    {
        return 1;
    }
    return 0;
}

int is_closure(struct token *token)
{
    if (token->type == EXPR_PARENTHESIS && strcmp(token->value, ")") == 0)
    {
        return 1;
    }
    return 0;
}

void infix_parcour(struct token *token, int *cr)
{
    if (token == NULL)
    {
        return;
    }

    if (is_action(token->type))
    {
        *cr = 1;
    }
    infix_parcour(token->left, cr);
    infix_parcour(token->right, cr);
    return;
}

int check_action(struct token *token)
{
    int cr = 0;
    if (token != NULL)
    {
        infix_parcour(token, &cr);
    }

    return cr;
}
