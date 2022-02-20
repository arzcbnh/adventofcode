// Advent of Code 2021 - Day 07
// Written by Henry Peaurt

/* While there's little information to keep track off, the calculations of optimal position and fuel consumption turned
 * out complicated */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strmanip.h"

// Types
typedef struct context {
	int *crabs_at;
	int max_position;
} context;

// Function declarations
void get_crabs(context *c);
long calc_fuel_use(context *c, _Bool mod);

int main(void)
{
	context c;
	c.crabs_at = NULL;
	c.max_position = 0;

	get_crabs(&c);

	printf("Part 1: %lu\n", calc_fuel_use(&c, false));
	printf("Part 2: %lu\n", calc_fuel_use(&c, true));

	free(c.crabs_at);

	return 0;
}

void get_crabs(context *c)
{
	char *s = str_input();
	int *crabs = NULL;
	int count = str_to_int_array(s, &crabs);
	int cap = 0;
	
	// It needs to initialize all allocated memory to 0, because not every position is occupied by a crab
	for (int i = 0; i < count; ++i) {
		int p = crabs[i];
		int m = c->max_position;

		if (cap <= p)
			c->crabs_at = realloc(c->crabs_at, sizeof(int) * (cap = p + 1));
		
		c->max_position = m > p ? m : p;
	}

	for (int i = 0; i <= c->max_position; ++i)
		c->crabs_at[i] = 0;

	while (count--)
		++c->crabs_at[crabs[count]];

	free(crabs);
}

long calc_fuel_use(context *c, _Bool mod)
{
	// Fuel consumption and optimal position have to be calculated together
	struct optimal {
		int pos;
		long fuel;
	} opt;
	opt.pos = 0;
	opt.fuel = (unsigned long) -1 >> 1;

	for (int i = 0; i <= c->max_position; ++i) {
		int fuel = 0;

		for (int j = 0; j <= c->max_position; ++j) {
			int dif = i - j > 0 ? i - j : j - i;
			fuel += (mod ? dif * (dif + 1) / 2 : dif) * c->crabs_at[j]; // Crazy ass formula
		}

		if (fuel < opt.fuel) {
			opt.pos = i;
			opt.fuel = fuel;
		}
	}

	return opt.fuel;
}
