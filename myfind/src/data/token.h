#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    EXPR_OR = 0,
    EXPR_AND,
    EXPR_NOT,
    EXPR_PARENTHESIS,
    EXPR_NAME,
    EXPR_TYPE,
    EXPR_NEWER,
    EXPR_USER,
    EXPR_GROUP,
    EXPR_PERM,
    EXPR_PRINT,
    EXPR_DELETE,
    EXPR_EXEC,
    EXPR_EXECDIR
};

struct token
{
    enum token_type type;
    struct token *left;
    struct token *right;
    char *value;
};

struct token *token_init(enum token_type, char *value);
struct token *token_cpy(struct token *to_copy);
void token_destroy(struct token *token);
#endif // TOKEN_H
