#include "free_buck.h"

void remove_bucket(struct bucket_list *bl, struct bucket *b)
{
    if (b == NULL || bl == NULL)
    {
        return;
    }
    if (bl->head == b)
    {
        if (b->next_bucket == NULL)
        {
            bl->head = NULL;
        }
        else
        {
            bl->head = b->next_bucket;
            b->next_bucket = NULL;
        }
    }
    else
    {
        struct bucket *current = bl->head;
        while (current->next_bucket != b)
        {
            current = current->next_bucket;
        }
        current->next_bucket = b->next_bucket;
        b->next_bucket = NULL;
    }

    return;
}
