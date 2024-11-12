#include <stddef.h>

#include "bucket.h"
#include "free_buck.h"
#include "utils.h"

struct bucket_list *bl = NULL;

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }
    if (bl == NULL)
    {
        bl = bucket_list_init(bl);
    }
    size_t aligned = align_size(size);
    struct bucket *b = find_available_bucket(bl, aligned);
    if (b == NULL)
    {
        b = bucket_create(bl, aligned);
        if (b == NULL)
        {
            return NULL;
        }
    }
    return alloc_my_block(b);
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    struct bucket *b = find_bucket(ptr);
    reset_flag(b, ptr);
    if (bucket_is_empty(b))
    {
        remove_bucket(bl, b);
        release_memory(b, b->b_size);
    }
    return;
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (size == 0)
    {
        return NULL;
    }
    struct bucket *b = find_bucket(ptr);
    if (b == NULL)
    {
        return malloc(size);
    }
    size_t aligned = align_size(size);
    if (aligned == b->block_size)
    {
        return ptr;
    }
    u_int8_t *new = malloc(aligned);
    u_int8_t i = 0;
    u_int8_t *castptr = ptr;
    while (i < b->block_size)
    {
        new[i] = castptr[i];
        i++;
    }
    free(ptr);
    void *new_ptr = new;
    return new_ptr;
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    if (nmemb == 0 || size == 0)
    {
        return NULL;
    }
    void *ptr = malloc(size * nmemb);
    u_int8_t *cast_ptr = ptr;
    size_t i = 0;
    while (i < nmemb * size)
    {
        *(cast_ptr + i) = 0;
        i++;
    }
    return ptr;
}
