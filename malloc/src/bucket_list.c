#include "bucket.h"
#include "utils.h"

void bucket_list_add(struct bucket_list *bl, struct bucket *new_bucket)
{
    if (bl == NULL || new_bucket == NULL)
    {
        return;
    }

    if (bl->head == NULL)
    {
        bl->head = new_bucket;
    }
    else
    {
        if (bl->head->block_size > new_bucket->block_size)
        {
            new_bucket->next_bucket = bl->head;
            bl->head = new_bucket;
        }
        else
        {
            struct bucket *current = bl->head;
            while (current->next_bucket
                   && current->next_bucket->block_size
                       <= new_bucket->block_size)
            {
                current = current->next_bucket;
            }

            new_bucket->next_bucket = current->next_bucket;
            current->next_bucket = new_bucket;
        }
    }
    return;
}

struct bucket_list *bucket_list_init(struct bucket_list *bl)
{
    bl = call_mmap(NULL, sysconf(_SC_PAGE_SIZE));
    bl->head = NULL;
    return bl;
}
