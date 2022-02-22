#include <stdio.h>

#include "memmanage.h"

int main(void)
{
	puts("mem_alloc...");
	for (int i = 0; i < 10; ++i) {
		int *ptr = mem_alloc(sizeof(int));
		if (ptr) *ptr = i + i;
	}

	puts("mem_realloc...");
	for (int i = 0; i < 10; ++i) {
		unsigned long *ptr = NULL;
		ptr = mem_realloc(ptr, i * sizeof(unsigned long));
		if (ptr) *ptr = i * i;
	}

	puts("mem_clean...");
	mem_clean();

	return 0;
}
