#include "../lib/lib.h"

void add_operator_in_operand(struct token *operator, struct stack * s_op)
{
    if (operator->type != EXPR_NOT)
    {
        struct token *right = token_cpy(stack_tail(s_op));
        stack_pop(s_op);
        operator->right = right;
    }
    struct token *left = token_cpy(stack_tail(s_op));
    stack_pop(s_op);
    operator->left = left;
    stack_add(s_op, operator);
    return;
}

// void print_stack(struct stack *s)
// {
//     printf("stack size %zu\n", s->size);
//     struct list *t = s->head;
//     while (t != NULL)
//     {
//         printf("type %d value %s\n", t->elt->type, t->elt->value);
//         t = t->next;
//     }
//     return;
// }

void flush_operator(struct stack *s_rator, struct stack *s_operand)
{
    while (s_rator->size > 0)
    {
        struct token *rator = token_cpy(stack_tail(s_rator));
        add_operator_in_operand(rator, s_operand);
        stack_pop(s_rator);
    }
    return;
}

int precedence(enum token_type type)
{
    switch (type)
    {
    case EXPR_AND:
        return 1;
    case EXPR_OR:
        return 2;
    case EXPR_NOT:
        return 3;
    default:
        return 0;
    }
}

void handle_priority(struct token *nt, struct stack *s_rator,
                     struct stack *s_op)
{
    while (s_rator->size > 0
           && precedence(nt->type) <= precedence(stack_tail(s_rator)->type))
    {
        struct token *rator = token_cpy(stack_tail(s_rator));
        add_operator_in_operand(rator, s_op);
        stack_pop(s_rator);
    }
    stack_add(s_rator, nt);
    return;
}

void depile_until_parenthesis(struct stack *s_rator, struct stack *s_op)
{
    while (s_rator->size > 0 && is_opener(stack_tail(s_rator)) == 0)
    {
        struct token *new_token = token_cpy(stack_tail(s_rator));
        add_operator_in_operand(new_token, s_op);
        stack_pop(s_rator);
    }
    stack_pop(s_rator);
    return;
}

void handle_parent(struct token *nt, struct stack *s_rator, struct stack *s_op)
{
    if (strcmp(nt->value, "(") == 0)
    {
        stack_add(s_rator, nt);
    }
    else
    {
        depile_until_parenthesis(s_rator, s_op);
    }
    return;
}

void add_and_operator_stack(struct stack *stack_operator, struct stack *s_op)
{
    enum token_type type = EXPR_AND;
    struct token *token = token_init(type, NULL);
    handle_priority(token, stack_operator, s_op);
    return;
}

void parse_args(struct queue *args, struct stack *s_rator, struct stack *s_op)
{
    int cpt = 0;
    while (args->size > 0)
    {
        struct token *next_args = token_cpy(queue_head(args));
        if (is_operator(next_args->type))
        {
            handle_priority(next_args, s_rator, s_op);
            cpt = 0;
        }
        else if (next_args->type == EXPR_PARENTHESIS)
        {
            if (cpt == 1 && is_opener(next_args))
            {
                add_and_operator_stack(s_rator, s_op);
            }
            handle_parent(next_args, s_rator, s_op);
            if (cpt == 1 && is_closure(next_args))
            {
                add_and_operator_stack(s_rator, s_op);
            }
            cpt = 0;
        }
        else
        {
            if (cpt == 1)
            {
                add_and_operator_stack(s_rator, s_op);
            }
            stack_add(s_op, next_args);
            cpt = 1;
        }
        queue_pop(args);
    }
    return;
}

struct token *adapt_ast(struct token *ast)
{
    struct token *new_token = token_init(EXPR_PRINT, NULL);
    if (ast != NULL)
    {
        struct token *new_root = token_init(EXPR_AND, NULL);
        new_root->left = ast;
        new_root->right = new_token;
        ast = new_root;
    }
    else
    {
        ast = new_token;
    }
    return ast;
}

struct token *build_ast(struct queue *args)
{
    struct stack *stack_operator = stack_init();
    struct stack *stack_operand = stack_init();
    parse_args(args, stack_operator, stack_operand);
    if (stack_operator->size > 0)
    {
        flush_operator(stack_operator, stack_operand);
    }
    queue_destroy(args);
    stack_destroy(stack_operator);
    struct token *ast = NULL;
    if (stack_operand->size > 0)
    {
        ast = token_cpy(stack_tail(stack_operand));
    }
    stack_destroy(stack_operand);
    if (check_action(ast) == 0)
    {
        ast = adapt_ast(ast);
    };
    return ast;
}
