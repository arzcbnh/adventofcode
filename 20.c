// Advent of Code 2021 - Day 20
// Written by Henry Peaurt

// Thankfully, I did this one before day 19. I'm not sure if it works for every possible input because of the algorithm
// I got. This time, i decided to keep the in a constant instead of scattering magic numbers everywhere.

#include <stdio.h>
#include <stdbool.h>

#include "strmanip.h"
#include "memmanage.h"

// Constants
#define BUF 51 // Default image buffer

// Types
typedef struct {
	char* algorithm;
	bool image[100 + BUF * 2][100 + BUF * 2];
	int size;
	int expanded;
} cntxt;

// Function declarations
void input_image(cntxt* c);
void enhance_image(cntxt* c);
int calc_pixel_value(cntxt* c, int x, int y);
void store_image(cntxt* c, bool copy[100 + BUF * 2][100 + BUF * 2]);
int count_pixels(cntxt* c);

int
main(void)
{
	cntxt c = {
		.algorithm = str_input(),
		.image = {{ 0 }},
		.size = 0,
		.expanded = 0
	};

	input_image(&c);

	for (int i = 0; i < 50; i++) {
		if (i == 2) {
			printf("Part 1: %i\n", count_pixels(&c));
		}

		enhance_image(&c);
	}

	printf("Part 2: %i\n", count_pixels(&c));

	mem_clean();

	return 0;
}

void
input_image(cntxt* c)
{
	getchar(); // Skip newline

	int y = BUF;
	char chr = getchar();

	do {
		for (int x = BUF; chr != '\n'; x++) {
			c->image[x][y] = chr == '#';
			chr = getchar();
		}

		y++;
		chr = getchar();
	} while (chr != EOF);

	c->size = y - BUF;
}

void
enhance_image(cntxt* c)
{
	bool copy[100 + BUF * 2][100 + BUF * 2] = { 0 };

	for (int y = 0; y < 100 + BUF * 2; y++) {
		for (int x = 0; x < 100 + BUF * 2; x++) {
			int index = calc_pixel_value(c, x, y);

			copy[x][y] = c->algorithm[index] == '#';
		}
	}

	store_image(c, copy);

	c->expanded++;
}

int
calc_pixel_value(cntxt* c, int x, int y)
{
	bool x_inside = 0 < x && x < 100 + BUF * 2 - 1;
	bool y_inside = 0 < y && y < 100 + BUF * 2 - 1;
	int value = 0;

	if (x_inside && y_inside) {
		value = c->image[x - 1][y - 1] << 1;
		value = (value | c->image[x][y - 1]) << 1;
		value = (value | c->image[x + 1][y - 1]) << 1;
		value = (value | c->image[x - 1][y]) << 1;
		value = (value | c->image[x][y]) << 1;
		value = (value | c->image[x + 1][y]) << 1;
		value = (value | c->image[x - 1][y + 1]) << 1;
		value = (value | c->image[x][y + 1]) << 1;
		value = (value | c->image[x + 1][y + 1]);
	} else {
		value = c->image[x][y] ? 511 : 0;
	}

	return value;
}

void
store_image(cntxt* c, bool copy[100 + BUF * 2][100 + BUF * 2])
{
	for (int y = 0; y < c->size + BUF * 2; y++) {
		for (int x = 0; x < c->size + BUF * 2; x++) {
			c->image[x][y] = copy[x][y];
		}
	}
}

int
count_pixels(cntxt* c)
{
	int count = 0;

	for (int y = 0; y < 100 + BUF * 2; y++) {
		for (int x = 0; x < 100 + BUF * 2; x++) {
			count += c->image[x][y];
		}
	}

	return count;
}
