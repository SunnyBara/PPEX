#include <stdio.h>

int main(void)
{
	void *ptr1 = malloc(2);
	void *ptr3 = malloc(17);
	void *ptr4 = malloc(6);
	printf("ptr1 b16 %p\n",ptr1);
	printf("ptr3 b32 %p\n",ptr3);
	printf("ptr4 b16 %p\n",ptr4);
    void *ptr5 = realloc(ptr1, 29);
	printf("ptr5 b32%p\n",ptr5);
	free(NULL);
	void *ptr2 = malloc(12);
	printf("ptr2 b16 %p\n",ptr2);
	free(ptr2);
	free(ptr4);
    free(ptr3);
    free(ptr5);
	return 0;
}
