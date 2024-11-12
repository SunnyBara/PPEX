#include "../lib/lib.h"

struct token *token_init(enum token_type type, char *value)
{
    struct token *token = malloc(sizeof(struct token));
    if (token == NULL)
    {
        return NULL;
    }
    token->type = type;
    token->left = NULL;
    token->right = NULL;
    token->value = value;

    return token;
}

struct token *token_cpy(struct token *to_copy)
{
    char *value = NULL;
    if (to_copy->value)
    {
        value = malloc(sizeof(char) * strlen(to_copy->value) + 1);
        strcpy(value, to_copy->value);
    }
    struct token *new = token_init(to_copy->type, value);
    new->left = to_copy->left;
    new->right = to_copy->right;
    return new;
}

void token_destroy(struct token *token)
{
    free(token->value);
    free(token);
    return;
}
