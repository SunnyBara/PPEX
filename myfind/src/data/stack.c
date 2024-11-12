#include "../lib/lib.h"

struct stack *stack_init(void)
{
    struct stack *stack = malloc(sizeof(struct stack));
    if (stack != NULL)
    {
        stack->tail = NULL;
        stack->size = 0;
    }
    return stack;
}

struct stack *stack_add(struct stack *stack, struct token *operande)
{
    if (stack == NULL)
    {
        return NULL;
    }
    struct list *new_item = list_init(operande);
    if (new_item == NULL)
    {
        return stack;
    }
    if (stack->size == 0)
    {
        stack->head = new_item;
        stack->tail = new_item;
    }
    else
    {
        stack->tail->next = new_item;
        stack->tail = new_item;
    }
    stack->size += 1;
    return stack;
}

struct token *stack_tail(struct stack *stack)
{
    return stack->tail->elt;
}

void stack_pop(struct stack *stack)
{
    if (stack == NULL || stack->size == 0)
    {
        return;
    }
    if (stack->size > 1)
    {
        struct list *tmp = stack->head;
        while (tmp->next != stack->tail)
        {
            tmp = tmp->next;
        }
        tmp->next = NULL;
        token_destroy(stack->tail->elt);
        free(stack->tail);
        stack->tail = tmp;
    }
    else
    {
        token_destroy(stack->tail->elt);
        free(stack->tail);
        stack->tail = NULL;
        stack->head = NULL;
    }
    stack->size -= 1;
    return;
}

struct stack *stack_clean(struct stack *stack)
{
    if (stack == NULL || stack->size == 0)
    {
        return NULL;
    }

    struct list *tmp = stack->head;
    struct list *save;
    while (tmp)
    {
        save = tmp;
        tmp = tmp->next;
        token_destroy(save->elt);
        free(save);
    }
    stack->head = NULL;
    stack->tail = NULL;
    stack->size = 0;
    return stack;
}

void stack_destroy(struct stack *stack)
{
    stack_clean(stack);
    free(stack);
    return;
}
