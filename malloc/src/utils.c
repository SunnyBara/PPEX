#define _GNU_SOURCE
#include "utils.h"

#include <stddef.h>

void *call_mmap(void *addr, size_t length)
{
    return mmap(addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
                -1, 0);
}

void release_memory(void *ptr, size_t block_size)
{
    munmap(ptr, block_size);
    return;
}
size_t page_needed(size_t size)
{
    size_t sizepage = sysconf(_SC_PAGE_SIZE);
    size_t nbrs = size / sizepage;
    if (size % sizepage != 0 || size == 0)
    {
        nbrs += 1;
    }
    return nbrs;
}

size_t find_number_of_block(size_t size, size_t sizeallocate)
{
    size_t size_left =
        sizeallocate - 2 * sizeof(size_t) - sizeof(struct bucket *);
    size_t i = 0;
    while (size_left > size)
    {
        size_left -= 1;
        size_left -= size;
        i++;
    }
    return i;
}
size_t align_size(size_t to_align)
{
    size_t start = 16;
    while (to_align > start)
    {
        start = start << 1;
    }
    return start;
}
