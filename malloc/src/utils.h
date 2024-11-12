#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

void *call_mmap(void *addr, size_t length);
size_t page_needed(size_t size);
size_t find_number_of_block(size_t size, size_t bucket);
size_t align_size(size_t to_align);
void release_memory(void *ptr, size_t block_size);
#endif // UTILS_H
