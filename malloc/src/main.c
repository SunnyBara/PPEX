#include <stdio.h>
#include <stdlib.h>

#include "test.h"
int main(void)
{
    void *ptr1 = my_malloc(2);
    void *ptr3 = my_malloc(17);
    void *ptr4 = my_malloc(6);
    printf("ptr1 b16 %p\n", ptr1);
    printf("ptr3 b32 %p\n", ptr3);
    printf("ptr4 b16 %p\n", ptr4);
    void *ptr5 = my_realloc(ptr1, 29);
    printf("ptr5 b32%p\n", ptr5);
    my_free(NULL);
    void *ptr2 = my_malloc(12);
    printf("ptr2 b16 %p\n", ptr2);
    my_free(ptr2);
    my_free(ptr4);
    my_free(ptr3);
    my_free(ptr5);
    return 0;
}
