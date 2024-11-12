#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#include "list.h"
#include "token.h"

struct stack
{
    struct list *head;
    struct list *tail;
    size_t size;
};

struct stack *stack_init(void);
struct stack *stack_add(struct stack *stack, struct token *operande);
struct token *stack_tail(struct stack *stack);
void stack_pop(struct stack *stack);
void stack_destroy(struct stack *stack);
#endif // STACK_H
