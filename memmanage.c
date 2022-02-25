// Heap management program for the Advent of Code 2021
// Written by Henry Peaurt

#include <stdlib.h>
#include <stdbool.h>

#include "memmanage.h"

// Types
typedef struct {
	size_t i;
	bool found;
} find_result;

// Global variables
static void **address_list;
static size_t qty;
static size_t cap;

static void
mem_expand(void)
{
	if (qty == cap)
		address_list = realloc(address_list, (cap += 1024) * sizeof(void*));
}

static find_result
mem_find(void *p)
{
	size_t i = p == NULL ? qty : 0;

	for (; i < qty; ++i) {
		if (p == address_list[i])
			break;
	}

	find_result fr = {
		.i = i,
		.found = i != qty ? true : false
	};

	return fr;
}

static void
mem_push(void *p)
{
	mem_expand();
	address_list[qty++] = p;
}

void*
mem_alloc(size_t n)
{
	if (n == 0) return NULL;

	void *p = calloc(n, sizeof(char));
	mem_push(p);
	return p;
}

void*
mem_realloc(void *p, size_t new_size)
{
	if (new_size == 0) return NULL;

	find_result fr = mem_find(p);

	if (fr.found) {
		p = realloc(p, new_size);
		address_list[fr.i] = p;
	} else
		p = mem_alloc(new_size);

	return p;
}
	
void
mem_clean(void)
{
	for (size_t i = 0; i < qty; ++i)
		free(address_list[i]);
	free(address_list);
}
