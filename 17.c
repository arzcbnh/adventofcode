// Advent of Code 2021 - Day 17
// Written by Henry Peaurt

// I confess to spoiling myself the Part 1 formula, but after trying some really complex math and realizing it's just
// n-sum and Bhaskara, I can't help but think this is one of the easiest programs to make. I just bruteforced
// everything into main and reorganized the program later. I don't know whether I feel thankful for how easy it was or
// annoyed because it was very boring. However, the program looks like crap and hard to understand.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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
bool check_landing(cntxt* c, int x_vel, int y_vel);

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
	printf("Part 2: %u\n", calc_shots(&c));

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
	int cnt = 0;

	for (int x_vel = sqrt(c->min_x * 4 + 1) / 2; x_vel <= c->max_x; x_vel++) {
		for (int y_vel = c->min_y; y_vel <= -c->min_y; y_vel++)
			cnt += check_landing(c, x_vel, y_vel);
	}

	return cnt;
}

bool
check_landing(cntxt* c, int x_vel, int y_vel)
{
	int x = 0;
	int y = 0;
	int curr_x_vel = x_vel;
	int curr_y_vel = y_vel;

	while (x <= c->max_x && y >= c->min_y) {
		if (c->min_x <= x && x <= c->max_x && c->min_y <= y && y <= c->max_y)
			return true;

		x += curr_x_vel;
		curr_x_vel -= curr_x_vel > 0;
		y += curr_y_vel;
		curr_y_vel--;
	}

	return false;
}
