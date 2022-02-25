// Header of heap management program for the Advent of Code 2021
// Written by Henry Peaurt

#ifndef MEM_MANAGE
#define MEM_MANAGE

// Functions
void *mem_alloc(size_t num);

void *mem_realloc(void *ptr, size_t new_size);
	
void mem_clean(void);

#endif
