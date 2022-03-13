// Heap management program for the Advent of Code 2021
// Written by Henry Peaurt

#include <stdlib.h>
#include <stdbool.h>

#include "memmanage.h"

// Types
typedef struct Cell {
	int id;
	void *address;
	size_t size;
	struct Cell *prev, *next;
} Cell;

// Global variables
static Cell *start, *end;

static Cell *
mem_find(void *p)
{
	Cell *s = start;
	Cell *e = end;

	while (s != NULL && e != NULL && s->id < e->id) {
		if (s == NULL || s->address == p) return s;
		s = s->prev;
		if (e == NULL || e->address == p) return e;
		e = e->prev;
		if (e == NULL || e->address == p) return e;
		e = e->prev;
	}

	return NULL;
}

static void
mem_push(Cell *c)
{
	if (start != NULL) {
		c->id = end->id + 1;
		c->prev = end;
		end->next = c;
		end = c;
	} else {
		c->id = 1;
		start = end = c;
	}
}

void *
mem_alloc(size_t n)
{
	if (n == 0) return NULL;

	Cell *c = calloc(1, sizeof(Cell));
	c->address = calloc(n, sizeof(char));
	c->size = n;
	mem_push(c);

	return c->address;
}

void *
mem_realloc(void *p, size_t new_size)
{
	if (p == NULL) return mem_alloc(new_size);
	if (new_size == 0) return NULL;

	Cell *c = mem_find(p);

	if (c != NULL && c->size < new_size) {
		c->address = realloc(c->address, new_size);
		c->size = new_size;
		return c->address;
	} else if (c == NULL)
		return mem_alloc(new_size);
	else // p is allocated and has more capacity than needed
		return p;
}
	
void
mem_clean(void)
{
	Cell *next;

	for (Cell *i = start; i != NULL; i = next) {
		next = i->next;
		free(i->address);
		free(i);
	}
}
