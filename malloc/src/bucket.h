#ifndef BUCKET_H
#define BUCKET_H

#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

struct bucket_list
{
    struct bucket *head;
};

struct bucket
{
    size_t b_size;
    size_t block_size;
    size_t block_nbrs;
    struct bucket *next_bucket;
    void *first_block;
    u_int8_t nbr_of_flag;
    u_int8_t last_flag_size;
    u_int8_t flag[];
};

union defaite
{
    size_t res;
    void *ptr;
};

struct bucket *bucket_create(struct bucket_list *bl, size_t size);
void bucket_list_add(struct bucket_list *bl, struct bucket *new_bucket);
struct bucket_list *bucket_list_init(struct bucket_list *bl);
struct bucket *find_available_bucket(struct bucket_list *bl, size_t size);
void *block_to_addr(struct bucket *b, u_int8_t block_number);
void *alloc_my_block(struct bucket *b);
u_int8_t addr_to_block(struct bucket *bucket, void *addr);
struct bucket *find_bucket(void *ptr);
void reset_flag(struct bucket *b, void *ptr);
int bucket_is_empty(struct bucket *b);
#endif /* !BUCKET_H */
