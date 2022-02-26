// Advent of Code 2021 - Day 17
// Written by Henry Peaurt

// I confess to spoiling myself the Part 1 formula, but after trying some really complex math and realizing it's so
// simple, I can't help but think this is one of the easiest programs to make. I just bruteforced everything into main
// and reorganized the program later.

#include <stdio.h>
#include <stdlib.h>

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
void input_target(cntxt* c);
int calc_shots(cntxt* c);

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

	printf("Part 1: %u\n", (-c.min_y - 1) * (-c.min_y) / 2);

	calc_shots(&c);

	return 0;
}

void
input_target(cntxt* c)
{
	char* s = str_input();

	c->min_x = atoi(str_word(3, s));
	c->max_x = atoi(str_word(4, s));
	c->min_y = - atoi(str_word(6, s));
	c->max_y = - atoi(str_word(7, s));
}

int
calc_shots(cntxt* c)
{
	int min_x_v = 0;
	int max_x_v = c->max_x;
	int min_y_v = c->min_y;
	int max_y_v = -c->min_y;
	int cnt = 0;

	while (min_x_v * (min_x_v + 1) / 2 < c->min_x)
		min_x_v++;

	for (int x = min_x_v; x <= max_x_v; x++) {
		for (int y = min_y_v; y <= max_y_v; y++) {
			int v_x = x;
			int v_y = y;
			int i = 0;
			int j = 0;

			while (i <= c->max_x && j >= c->min_y) {
				if (c->min_x <= i && i <= c->max_x && c->min_y <= j && j <= c->max_y) {
				printf("%i %i\n", x, y);
					++cnt;
					break;
				}

				i += v_x;
				v_x -= v_x > 0;
				j += v_y;
				v_y--;
			}
		}
	}

	printf("%i %i %i %i %i\n", min_x_v, max_x_v, min_y_v, max_y_v, cnt);

	return 0;
}
