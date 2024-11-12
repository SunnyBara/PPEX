#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#include "list.h"
#include "token.h"

struct queue
{
    struct list *head;
    struct list *tail;
    size_t size;
};

void queue_add(struct queue *queue, struct token *elt);
struct token *queue_head(struct queue *queue);
void queue_pop(struct queue *queue);
struct queue *queue_init(void);
void queue_destroy(struct queue *queue);
#endif // QUEUE_H
