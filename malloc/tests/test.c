#include <criterion/assert.h>
#include <criterion/criterion.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/bucket.h"
#include "../src/utils.h"
static u_int8_t set_one_bit(u_int8_t *flag, u_int8_t mask)
{
    u_int8_t res = *flag + mask;
    return res;
}

static u_int8_t flag_to_pos(u_int8_t flag)
{
    u_int8_t mask = 128;
    u_int8_t i = 0;
    while (mask > flag)
    {
        mask = mask >> 1;
        i++;
    }
    return i;
}

static u_int8_t block_pos(struct bucket *b)
{
    u_int8_t i = 0;
    while (i < b->nbr_of_flag)
    {
        if (b->flag[i] != 0)
        {
            return (flag_to_pos(b->flag[i]) + 8 * i);
        }
        i++;
    }
    return 0;
}

static bool check_flag(struct bucket *b)
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
    if (b->block_size > size)
    {
        return -1;
    }
    else if (b->block_size < size)
    {
        return 0;
    }
    return check_flag(b);
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
    return i;
}

Test(bucket_list, test_init)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    cr_expect(&bl != NULL, "Allocation failed");
}

Test(bucket_list, test_add_bucket)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    struct bucket *b1 = malloc(sizeof(struct bucket));
    struct bucket *b2 = malloc(sizeof(struct bucket));
    struct bucket *b3 = malloc(sizeof(struct bucket));
    b1->block_size = 8;
    b1->next_bucket = NULL;
    b2->block_size = 16;
    b2->next_bucket = NULL;
    b3->block_size = 8;
    b3->next_bucket = NULL;
    bucket_list_add(&bl, b1);
    bucket_list_add(&bl, b2);
    bucket_list_add(&bl, b3);
    size_t block_b1 = bl.head->block_size;
    size_t block_b3 = bl.head->next_bucket->block_size;
    size_t block_b2 = bl.head->next_bucket->next_bucket->block_size;

    cr_expect(block_b1 == b1->block_size, "Expected %ld got %ld",
              b2->block_size, block_b1);
    cr_expect(block_b2 == b2->block_size, "Expected %ld got %ld",
              b3->block_size, block_b2);
    cr_expect(block_b3 == b3->block_size, "Expected %ld got %ld",
              b1->block_size, block_b3);
    cr_expect(bl.head = b1, "Expected b as head got %p", bl.head);
    cr_expect(bl.head->next_bucket = b3, "Expected b3 as head next got %p",
              bl.head->next_bucket);
    cr_expect(b3->next_bucket == b2, "Expected b2 as b3->next got %p",
              b3->next_bucket);
    cr_expect(b1->next_bucket == b3, "Expected b3 as b->next got %p",
              b1->next_bucket);
}

Test(utils, aligned)
{
    size_t size_to_align = 2;
    size_t aligned = align_size(size_to_align);
    cr_expect(aligned == 16, "Expected 16 got %zu", aligned);
    size_to_align = 8;
    aligned = align_size(size_to_align);
    cr_expect(aligned == 16, "Expected 16 got %zu", aligned);
    size_to_align = 12;
    aligned = align_size(size_to_align);
    cr_expect(aligned == 16, "Expected 16 got %zu", aligned);
    size_to_align = 30;
    aligned = align_size(size_to_align);
    cr_expect(aligned == 32, "Expected 32 got %zu", aligned);
    size_to_align = 0;
    aligned = align_size(size_to_align);
    cr_expect(aligned == 16, "Expected 16 got %zu", aligned);
}

Test(utils, find_number_of_block)
{
    size_t size = align_size(sysconf(_SC_PAGE_SIZE));
    size_t nbr_block = find_number_of_block(size, sysconf(_SC_PAGE_SIZE));
    cr_expect(nbr_block == 0, "EXpected 0 got %zu", nbr_block);
    size = align_size(sysconf(_SC_PAGE_SIZE) / 2);
    nbr_block = find_number_of_block(size, sysconf(_SC_PAGE_SIZE));
    cr_expect(nbr_block == 1, "EXpected 1 got %zu", nbr_block);
    size = align_size(sysconf(_SC_PAGE_SIZE) / 3);
    nbr_block = find_number_of_block(size, sysconf(_SC_PAGE_SIZE));
    cr_expect(nbr_block == 1, "EXpected 1 got %zu", nbr_block);
    size = align_size(sysconf(_SC_PAGE_SIZE) / 4);
    nbr_block = find_number_of_block(size, sysconf(_SC_PAGE_SIZE));
    cr_expect(nbr_block == 3, "EXpected 3 got %zu", nbr_block);
}

Test(bucket, flag_init)
{
    struct bucket *b1 = call_mmap(NULL, sysconf(_SC_PAGE_SIZE));
    b1->block_size = 16;
    b1->block_nbrs = 54;
    u_int8_t n = flag_init(b1, b1->block_nbrs);
    cr_expect(n == 7, "Expected 7 got %d", n);
    cr_expect(b1->flag[0] == 255, "expected 255 got %d", b1->flag[0]);
    cr_expect(b1->flag[n - 1] == 252, "expected 252 got %d", b1->flag[6]);
    b1->block_size = 16;
    b1->block_nbrs = 3;
    n = flag_init(b1, b1->block_nbrs);
    cr_expect(n == 1, "Expected 1 got %d", n);
    cr_expect(b1->flag[n - 1] == 128 + 64 + 32, "expected 226  got %d",
              b1->flag[0]);
}

Test(bucket, check_flag)
{
    struct bucket *b1 = call_mmap(NULL, sysconf(_SC_PAGE_SIZE));
    b1->block_size = 16;
    b1->block_nbrs = 54;
    u_int8_t n = flag_init(b1, b1->block_nbrs);
    b1->nbr_of_flag = n;
    bool res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[0] = 0;
    res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[0] = 0;
    res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[1] = 0;
    res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[2] = 0;
    res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[3] = 0;
    res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[4] = 0;
    res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[5] = 0;
    res = check_flag(b1);
    cr_expect(res == 1, "expected 1 got %d", res);
    b1->flag[6] = 0;
    res = check_flag(b1);
    cr_expect(res == 0, "expected 0 got %d", res);
}

Test(bucket, a_block_is_available)
{
    struct bucket *b1 = call_mmap(NULL, sysconf(_SC_PAGE_SIZE));
    b1->block_size = 16;
    b1->block_nbrs = 54;
    u_int8_t n = flag_init(b1, b1->block_nbrs);
    b1->nbr_of_flag = n;
    int res = a_block_is_available(b1, 16);
    cr_expect(res == 1, "expected 1 got %d", res);
    res = a_block_is_available(b1, 8);
    cr_expect(res == -1, "expected -1 got %d", res);
    res = a_block_is_available(b1, 32);
    cr_expect(res == 0, "expected 0 got %d", res);
}

Test(bucket, find_available)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    struct bucket *b1 = call_mmap(NULL, sysconf(_SC_PAGE_SIZE));
    b1->block_size = 16;
    b1->block_nbrs = 54;
    u_int8_t n = flag_init(b1, b1->block_nbrs);
    b1->nbr_of_flag = n;

    struct bucket *res = find_available_bucket(&bl, 16);
    cr_expect(res == NULL, "expected NULL got %p", res);
    bl.head = b1;

    res = find_available_bucket(&bl, 16);
    cr_expect(res == b1, "expected b1 got %p", res);

    res = find_available_bucket(&bl, 8);
    cr_expect(res == NULL, "expected NULL got %p", res);

    struct bucket *b2 = call_mmap(NULL, sysconf(_SC_PAGE_SIZE));
    b2->block_size = 32;
    b2->block_nbrs = 54;
    n = flag_init(b2, b2->block_nbrs);
    b2->nbr_of_flag = n;
    b1->next_bucket = b2;
    res = find_available_bucket(&bl, 32);
    cr_expect(res == b2, "expected b2 got %p", res);
    res = find_available_bucket(&bl, 64);
    cr_expect(res == NULL, "expected NULL got %p", res);
}

Test(bucket, create_bucket)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    struct bucket *b = bucket_create(&bl, 8);
    cr_expect(b->block_size == 8, "expected block size 8 got %zu",
              b->block_size);
    cr_expect(b->flag[0] == 255, "expected flag value 255 got %zu", b->flag[0]);
    cr_expect(bl.head = b, "Expected b as head got %p", bl.head);
    struct bucket *b2 = bucket_create(&bl, 16);
    cr_expect(b2->block_size == 16, "expected block size 16 got %zu",
              b2->block_size);
    cr_expect(b2->flag[0] == 255, "expected flag value 255 got %zu",
              b2->flag[0]);
    cr_expect(bl.head = b, "Expected b as head got %p", bl.head);
    cr_expect(bl.head->next_bucket = b2, "Expected b2 as head next got %p",
              bl.head->next_bucket);
    cr_expect(b->next_bucket == b2, "Expected b2 as b->next got %p",
              b->next_bucket);
    struct bucket *b3 = bucket_create(&bl, 8);
    cr_expect(b3->block_size == 8, "expected block size 8 got %zu",
              b3->block_size);
    cr_expect(b3->flag[0] == 255, "expected flag value 255 got %zu",
              b3->flag[0]);
    cr_expect(bl.head = b, "Expected b as head got %p", bl.head);
    cr_expect(bl.head->next_bucket = b3, "Expected b3 as head next got %p",
              bl.head->next_bucket);
    cr_expect(b3->next_bucket == b2, "Expected b2 as b3->next got %p",
              b3->next_bucket);
    cr_expect(b->next_bucket == b3, "Expected b3 as b->next got %p",
              b->next_bucket);
    struct bucket *b4 = bucket_create(&bl, 32);
    cr_expect(b2->next_bucket == b4, "expected b4 as b2 next");
    struct bucket *b5 = bucket_create(&bl, 4);
    cr_expect(bl.head == b5, "Expected new head");
}

Test(bucket, flag_to_pos)
{
    u_int8_t f = 255;
    u_int8_t res = flag_to_pos(f);
    cr_expect(res == 0, "expected 0 got %lu", res);
    f = 143;
    res = flag_to_pos(f);
    cr_expect(res == 0, "expected 0 got %lu", res);
    f = 78;
    res = flag_to_pos(f);
    cr_expect(res == 1, "expected 1 got %lu", res);
    f = 56;
    res = flag_to_pos(f);
    cr_expect(res == 2, "expected 2 got %lu", res);
    f = 13;
    res = flag_to_pos(f);
    cr_expect(res == 4, "expected 4 got %lu", res);
    f = 1;
    res = flag_to_pos(f);
    cr_expect(res == 7, "expected 7 got %lu", res);
}

Test(bucket, get_block_pos)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    struct bucket *b = bucket_create(&bl, 8);
    u_int8_t res = block_pos(b);
    cr_expect(res == 0, "Expected 0 got %lu", res);
    b->flag[0] = 0;
    b->flag[1] = 135;
    res = block_pos(b);
    cr_expect(res == 8, "Expected 8 got %lu", res);
    b->flag[1] = 13;
    res = block_pos(b);
    cr_expect(res == 12, "Expected 12 got %lu", res);
}

Test(bucket, alloc_my_block)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    size_t size = 4;
    size_t aligned = align_size(size);
    struct bucket *b = find_available_bucket(&bl, aligned);
    if (b == NULL)
    {
        b = bucket_create(&bl, aligned);
    }
    cr_expect(b->flag[0] == 255, "expected 255 got %lu", b->flag[0]);
    void *res = alloc_my_block(b);
    u_int8_t addr_uint = addr_to_block(b, res);
    void *addr = block_to_addr(b, addr_uint);
    void *cast = b->first_block;
    cr_expect(b->flag[0] == 127, "expected 127 got %lu", b->flag[0]);
    cr_expect(res == cast, "Expected %p got %p", res, cast);
    cr_expect(res == addr, "Expected %p got %p", addr, res);
}

Test(bucket, find_bucket)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    struct bucket *b = bucket_create(&bl, 8);
    struct bucket *b2 = bucket_create(&bl, 16);
    struct bucket *b3 = bucket_create(&bl, 8);
    b3->block_size = 8;
    size_t size = 9;
    size_t aligned = align_size(size);
    struct bucket *bf = find_available_bucket(&bl, aligned);
    if (b == NULL)
    {
        bf = bucket_create(&bl, aligned);
    }
    void *res = alloc_my_block(bf);
    struct bucket *save = find_bucket(res);
    cr_expect(save == b2, " Expected %p got %p", b2, save);
}

Test(bucket, set_bit_one)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    struct bucket *b = bucket_create(&bl, 8);
    alloc_my_block(b);
    b->flag[0] = set_one_bit(b->flag, 128);
    cr_expect(b->flag[0] == 255, "Expected 255 got %lu", b->flag[0]);
}

uintptr_t cast_ptr(void *ptr)
{
    return (uintptr_t)ptr;
}

Test(bucket, reset_flag)
{
    struct bucket_list bl;
    bucket_list_init(&bl);
    struct bucket *b = bucket_create(&bl, 8);
    void *ptr1 = alloc_my_block(b);
    void *ptr2 = alloc_my_block(b);
    uintptr_t casteptr = cast_ptr(b->first_block);
    printf("first block %p\n endofflag %p\n", b->first_block,
           (void *)(b->flag + b->nbr_of_flag));
    u_int8_t res = casteptr % 16;
    cr_expect(res == 0, "Expected 0 got %u", res);
    cr_expect(b->flag[0] == 63, "Expected 63 got %lu", b->flag[0]);
    reset_flag(b, ptr2);
    cr_expect(b->flag[0] == 127, "Expected 127 got %lu", b->flag[0]);
    reset_flag(b, ptr1);
    cr_expect(b->flag[0] == 255, "Expected 255 got %lu", b->flag[0]);
}

Test(bucket, aligner)
{
    struct bucket_list bl = { .head = NULL };
    size_t aligned = align_size(18);
    struct bucket *b = bucket_create(&bl, aligned);
    void *ptr1 = alloc_my_block(b);
    void *ptr2 = alloc_my_block(b);
    uintptr_t cast_ptr1 = cast_ptr(ptr1);
    uintptr_t cast_ptr2 = cast_ptr(ptr2);
    printf("ptr1 %p\nptr2 %p\n", ptr1, ptr2);
    cr_expect(b->next_bucket == NULL,
              "Expected NULL mais t en a creer un autre");
    cr_expect(cast_ptr1 % 16 == 0, "ptr1 not aligned");
    cr_expect(cast_ptr2 % 16 == 0, "ptr2 not aligned");
    cr_expect(cast_ptr1 + 32 == cast_ptr2, "expected %lu got %lu", cast_ptr2,
              cast_ptr1 + 16);
}
