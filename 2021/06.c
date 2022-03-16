// Advent of Code 2021 - Day 06
// Written by Henry Peaurt

/* Really simple */

#include <stdio.h>
#include <stdlib.h>

#include "strmanip.h"

// Function declarations
void get_fish(unsigned long *f);
void step_fish(unsigned long *f);
unsigned long sum_fish(unsigned long *f);

int main(void)
{
	unsigned long fish[9] = { 0 };

	get_fish(fish);

	for (int i = 0; i < 256; ++i) {
		if (i == 80)
			printf("Part 1: %lu\n", sum_fish(fish));

		step_fish(fish);
	}

	printf("Part 2: %lu\n", sum_fish(fish));

	return 0;
}

void get_fish(unsigned long *f)
{
	char *s = str_input();
	unsigned long len = strlen(s);

	for (unsigned int i = 0; i < len; i += 2)
		++f[s[i] - '0'];

	free(s);
}

void step_fish(unsigned long *f)
{
	unsigned long tmp = f[0];

	for (int i = 0; i < 8; ++i)
		f[i] = f[i + 1];

	f[6] += tmp;
	f[8] = tmp;
}

unsigned long sum_fish(unsigned long *f)
{
	unsigned long n = 0;

	for (int i = 0; i < 9; ++i)
		n += f[i];

	return n;
}
