// Advent of Code 2021 - Day 01
// Written by Henry Peaurt

/* It's a little bit complex, but I think processing the input by line is quick and easy enough */

#include <stdio.h>
#include <stdlib.h>

#include "strmanip.h"

int main(void)
{
	char *s = str_input();
	int very_prev = atoi(s);
	free(s);

	s = str_input();
	int prev = atoi(s);
	free(s);

	int incr1 = 0;
	int incr2 = 0;
	int prev_sum = (unsigned) -1 >> 1;

	if (very_prev < prev)
		++incr1;

	while ((s = str_input())) {
		puts(s);
		int curr = atoi(s);
		free(s);

		int curr_sum = very_prev + prev + curr;
		if (prev_sum < curr_sum)
			++incr2;
		prev_sum = curr_sum;

		if (prev < curr)
			++incr1;
		very_prev = prev;
		prev = curr;
	}

	printf("Part 1: %d\n", incr1);
	printf("Part 2: %d\n", incr2);
	return 0;
}
