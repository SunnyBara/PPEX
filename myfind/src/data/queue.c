#include "../lib/lib.h"

size_t queue_size(struct queue *queue)
{
    return queue->size;
}

void queue_add(struct queue *queue, struct token *elt)
{
    if (queue != NULL)
    {
        struct list *new_elt = list_init(elt);
        if (queue->tail != NULL)
        {
            queue->tail->next = new_elt;
        }
        if (queue->head == NULL)
        {
            queue->head = new_elt;
        }
        queue->tail = new_elt;
        queue->size += 1;
    }
    return;
}

struct token *queue_head(struct queue *queue)
{
    return queue->head->elt;
}

void queue_pop(struct queue *queue)
{
    if (queue->head != NULL)
    {
        struct list *tmp = queue->head;
        queue->head = queue->head->next;
        if (queue->head == NULL)
        {
            queue->tail = NULL;
        }
        token_destroy(tmp->elt);
        free(tmp);
        queue->size -= 1;
    }
    return;
}

struct queue *queue_init(void)
{
    struct queue *queue = malloc(sizeof(struct queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void queue_clear(struct queue *queue)
{
    if (queue != NULL)
    {
        struct list *tmp = queue->head;
        while (queue->head->next != NULL)
        {
            queue->head = queue->head->next;
            token_destroy(tmp->elt);
            free(tmp);
            tmp = queue->head;
        }
        token_destroy(tmp->elt);
        free(tmp);
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
    }
    return;
}

void queue_destroy(struct queue *queue)
{
    if (queue->size > 0)
    {
        queue_clear(queue);
    }
    free(queue);
    return;
}
