// Heap management header for the Advent of Code 2021
// Written by Henry Peaurt

#ifndef MEM_MANAGE
#define MEM_MANAGE

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	unsigned long i;
	_Bool found;
} mem_result;

// Doesn't make sense to make it static because it's a header, but whatever
static void **mem_add;
static unsigned long qty;
static unsigned long cap;

static void mem_expand()
{
	if (qty == cap)
		mem_add = realloc(mem_add, (cap += 1024) * sizeof(void*));
}

mem_result mem_find(void *ptr)
{
	unsigned long i = 0;

	if (!ptr) i = qty;

	for (; i < qty; ++i) {
		if (ptr == mem_add[i])
			break;
	}

	mem_result r = {
		.i = i,
		.found = i != qty ? true : false
	};

	return r;
}

void mem_push(void *ptr)
{
	mem_expand();
	mem_add[qty++] = ptr;
}

void *mem_alloc(size_t num)
{
	if (!num) return NULL;

	void *ptr = calloc(num, sizeof(char));
	mem_push(ptr);
	return ptr;
}

void *mem_realloc(void *ptr, size_t new_size)
{
	if (!new_size) return NULL;

	mem_result m = mem_find(ptr);

	if (m.found) {
		ptr = realloc(ptr, new_size);
		mem_add[m.i] = ptr;
	} else
		ptr = mem_alloc(new_size);

	return ptr;
}
	
void mem_clean(void)
{
	for (unsigned long i = 0; i < qty; ++i)
		free(mem_add[i]);
	free(mem_add);
}

#endif
