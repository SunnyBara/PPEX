#include "bucket.h"

#include <stdint.h>

#include "utils.h"

void *block_to_addr(struct bucket *b, u_int8_t block_number)
{
    u_int8_t *addr_of_last_flag = b->first_block;
    u_int8_t *addr_wanted = addr_of_last_flag + (block_number * b->block_size);
    void *cast_addr = addr_wanted;
    return cast_addr;
}

u_int8_t addr_to_block(struct bucket *bucket, void *addr)
{
    u_int8_t *addr_of_last_flag = bucket->first_block;
    u_int8_t *cast_addr = addr;

    return ((cast_addr - addr_of_last_flag) / bucket->block_size);
}

static u_int8_t set_partial_flag(size_t nbr)
{
    size_t i = 0;
    u_int8_t res = 128;
    while (i < nbr - 1)
    {
        res = res >> 1;
        res += 128;
        i++;
    }
    return res;
}

static u_int8_t flag_init(struct bucket *buck, size_t nbr)
{
    size_t i = 0;
    while (nbr > 8)
    {
        buck->flag[i] = 255;
        nbr -= 8;
        i++;
    }
    if (nbr > 0)
    {
        buck->flag[i] = set_partial_flag(nbr);
        i++;
    }
    buck->last_flag_size = buck->flag[i - 1];
    return i;
}

static uintptr_t cast_void_to_uint(void *ptr)
{
    return (uintptr_t)ptr;
}

static void *align_first_ptr(struct bucket *b)
{
    u_int8_t *ptr = b->flag;
    u_int8_t mod = 16;
    ptr += b->nbr_of_flag;
    uintptr_t casted = cast_void_to_uint(ptr);
    if (casted % mod != 0)
    {
        u_int8_t shift = mod - (casted % mod);
        ptr += shift;
    }
    return ptr;
}

struct bucket *bucket_create(struct bucket_list *bl, size_t size)
{
    if (bl == NULL)
    {
        return NULL;
    }
    size_t nbr_page = page_needed(size);
    size_t size_to_alloc = nbr_page * sysconf(_SC_PAGE_SIZE);
    struct bucket *buck = call_mmap(NULL, size_to_alloc);
    if (buck == MAP_FAILED)
    {
        return NULL;
    }
    size_t number_of_block = find_number_of_block(size, size_to_alloc);
    buck->b_size = size_to_alloc;
    buck->block_nbrs = number_of_block;
    buck->block_size = size;
    buck->next_bucket = NULL;
    buck->nbr_of_flag = flag_init(buck, number_of_block);
    buck->first_block = align_first_ptr(buck);
    bucket_list_add(bl, buck);
    return buck;
}

static int check_flag(struct bucket *b)
{
    u_int8_t i = 0;
    while (i < b->nbr_of_flag)
    {
        if (b->flag[i] != 0)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

static int a_block_is_available(struct bucket *b, size_t size)
{
    if (b == NULL || b->block_size > size)
    {
        return -1;
    }
    else if (b->block_size < size)
    {
        return 0;
    }
    return check_flag(b);
}

struct bucket *find_available_bucket(struct bucket_list *bl, size_t size)
{
    if (bl->head == NULL)
    {
        return NULL;
    }
    struct bucket *current = bl->head;
    int cr = a_block_is_available(current, size);
    while (current != NULL && cr == 0)
    {
        current = current->next_bucket;
        cr = a_block_is_available(current, size);
    }
    if (cr == -1)
    {
        return NULL;
    }
    return current;
}

static u_int8_t set_zero_bit(u_int8_t flag, u_int8_t mask)
{
    u_int8_t res = flag & (~mask);
    return res;
}

static u_int8_t set_one_bit(u_int8_t flag, u_int8_t mask)
{
    u_int8_t res = flag + mask;
    return res;
}

struct bucket *find_bucket(void *ptr)
{
    if (ptr == NULL)
    {
        return NULL;
    }
    size_t shifter = sysconf(_SC_PAGE_SIZE);
    size_t mask = ~(shifter - 1);
    size_t res = mask & (size_t)ptr;
    union defaite rip = { .res = res };
    return rip.ptr;
}

static u_int8_t flag_to_pos(u_int8_t *flag, u_int8_t i)
{
    u_int8_t mask = 128;
    u_int8_t j = 0;
    while (mask > flag[i])
    {
        mask = mask >> 1;
        j++;
    }
    flag[i] = set_zero_bit(flag[i], mask);
    return j;
}

static u_int8_t get_block_pos(struct bucket *b)
{
    u_int8_t i = 0;
    while (i < b->nbr_of_flag)
    {
        if (b->flag[i] != 0)
        {
            return (flag_to_pos(b->flag, i) + 8 * i);
        }
        i++;
    }
    return 0;
}

void reset_flag(struct bucket *b, void *ptr)
{
    u_int8_t slot = addr_to_block(b, ptr);
    u_int8_t flag_num = slot / 8;
    u_int8_t flag_pos = slot % 8;
    u_int8_t mask = 128 >> flag_pos;
    b->flag[flag_num] = set_one_bit(b->flag[flag_num], mask);
}

void *alloc_my_block(struct bucket *b)
{
    u_int8_t block_pos = get_block_pos(b);
    return block_to_addr(b, block_pos);
}

int bucket_is_empty(struct bucket *b)
{
    u_int8_t i = 0;
    while (i < b->nbr_of_flag - 1)
    {
        if (b->flag[i] != 255)
        {
            return 0;
        }
        i++;
    }
    if (b->flag[i] != b->last_flag_size)
    {
        return 0;
    }
    return 1;
}
