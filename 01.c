// Advent of Code 2021 - Day 01
// Written by Henry Peaurt

/* It's a little bit complex, but I think processing the input by line is quick and easy enough */

#include <stdio.h>
#include <stdlib.h>

#include "strmanip.h"
#include "memmanage.h"

int main(void)
{
	int anteprev = atoi(str_input());
	int prev = atoi(str_input());
	int curr = atoi(str_input());

	int part1 = (anteprev < prev) + (prev < curr);
	int part2 = 0;

	int prev_sum = anteprev + prev + curr;
	char *s;

	while (s = str_input()) {
		anteprev = prev;
		prev = curr;
		curr = atoi(s);

		int curr_sum = anteprev + prev + curr;

		part1 += prev < curr;
		part2 += prev_sum < curr_sum;

		prev_sum = curr_sum;
	}

	printf("Part 1: %u\n", part1);
	printf("Part 2: %u\n", part2);

	mem_clean();

	return 0;
}
