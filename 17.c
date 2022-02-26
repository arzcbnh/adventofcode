// Advent of Code 2021 - Day 17
// Written by Henry Peaurt

// Really wish I had my Physics degree already

#include <stdio.h>
#include <string.h>

#include "strmanip.h"
#include "memmanage.h"

// Types
typedef struct {
	int min_x;
	int max_x;
	int min_y;
	int max_y;
} cntxt;
// Function declarations

int
main(void)
{
	cntxt c = {
		.min_x = 0,
		.max_x = 0,
		.min_y = 0,
		.max_y = 0
	};

	input_target(&c);

	printf("%i %i %i %i\n", c.min_x, c.max_x, c.min_y, c.max_y);
	return 0;
}

void
input_target(cntxt* c)
{
	char* s = str_input();

	c->min_x = atoi(str_word(s, 3));
	c->max_x = atoi(str_word(s, 4));
	c->min_y = - atoi(str_word(s, 6));
	c->max_y = - atoi(str_word(s, 7));
}
