// Advent of Code 2021 - Day 09
// Written by Henry Peaurt

/* I'm quite proud of this one. I think I was able to divide functions really well, keeping them small and unique. I
 * also used arrays and only now found out you don't need to declare the struct key for a typedef. I'm just not sure if
 * using y before x in the array definition was a good idea, it caused some bugs that left me fumbling for an hour.
 * I'll keep that in mind for my next n-dimensional arrays */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strmanip.h"

// Types
typedef struct {
	int x;
	int y;
} point;

typedef struct {
	int map[100][100];
	int size;

	point **lowp;
	int lowp_count;

	point **measured;
	int measured_count;

	int largest[3];
} context;

// Function declarations
void get_map(context *c);
_Bool check_lowpoint(context *c, int x, int y);
void store_lowpoint(context *c, int x, int y);
long calc_risk(context *c);
int measure_basin(context *c, int x, int y);
_Bool check_measured(context *c, int x, int y);
void store_measured(context *c, int x, int y);
void compare_basins(int largest[3], int size);

int main(void)
{
	context c = {
		.map = { { 0, 0 } },
		.size = 0,

		.lowp = NULL,
		.lowp_count = 0,

		.measured = NULL,
		.measured_count = 0,

		.largest = { 0 }
	};

	get_map(&c);

	for (int y = 0; y < c.size; ++y) {
		for (int x = 0; x < c.size; ++x) {
			if (check_lowpoint(&c, x, y))
				store_lowpoint(&c, x, y);
		}
	}

	printf("Part 1: %lu\n", calc_risk(&c));

	for (int i = 0; i < c.lowp_count; ++i) {
		int x = c.lowp[i]->x;
		int y = c.lowp[i]->y;

		compare_basins(c.largest, measure_basin(&c, x, y));
	}

	printf("Part 2: %lu\n", (unsigned long) (c.largest[0] * c.largest[1] * c.largest[2]));

	return 0;
}

void get_map(context *c)
{
	char *s = str_input();
	int size = strlen(s);
	int y = 0;
	
	c->size = size;

	do { 
		for (int x = 0; x < size; ++x)
			c->map[y][x] = s[x] - '0';

		++y;
		free(s);
	} while ((s = str_input()));
}

_Bool check_lowpoint(context *c, int x, int y)
{
	int (*map)[100][100] = &(c->map);
	int sz = c->size;
	int p = (*map)[y][x];
	_Bool up = true;
	_Bool left = true;
	_Bool down = true;
	_Bool right = true;

	if (y > 0) up = p < (*map)[y - 1][x];
	if (x > 0) left = p < (*map)[y][x - 1];
	if (y < sz - 1) down = p < (*map)[y + 1][x];
	if (x < sz - 1) right = p < (*map)[y][x + 1];

	return up && left && down && right;
}

void store_lowpoint(context *c, int x, int y)
{
	static int cap;

	point *p = malloc(sizeof(point));
	p->x = x;
	p->y = y;

	if (c->lowp_count == cap)
		c->lowp = realloc(c->lowp, sizeof(point*) * (cap += 256));

	c->lowp[c->lowp_count++] = p;
}

long calc_risk(context *c)
{
	long sum = 0;

	for (int i = 0; i < c->lowp_count; ++i) {
		int x = c->lowp[i]->x;
		int y = c->lowp[i]->y;

		sum += c->map[y][x] + 1;
	}

	return sum;
}

int measure_basin(context *c, int x, int y)
{
	int (*map)[100][100] = &(c->map);
	int sz = c->size;
	int p = (*map)[y][x];
	int measure = 0;

	if (p == 9 || check_measured(c, x, y))
		return 0;

	store_measured(c, x, y);

	if (x > 0 && (*map)[y][x - 1] > p) measure += measure_basin(c, x - 1, y);			
	if (y > 0 && (*map)[y - 1][x] > p) measure += measure_basin(c, x, y - 1);
	if (x < sz - 1 && (*map)[y][x + 1] > p) measure += measure_basin(c, x + 1, y);
	if (y < sz - 1 && (*map)[y + 1][x] > p) measure += measure_basin(c, x, y + 1);

	return measure + 1;
}

_Bool check_measured(context *c, int x, int y)
{
	for (int i = 0; i < c->measured_count; ++i) {
		if (c->measured[i]->x == x && c->measured[i]->y == y)
			return true;
	}

	return false;
}

void store_measured(context *c, int x, int y)
{
	static int cap;

	point *p = malloc(sizeof(point));
	p->x = x;
	p->y = y;

	if (c->measured_count == cap)
		c->measured = realloc(c->measured, sizeof(point*) * (cap += 256));

	c->measured[c->measured_count++] = p;
}

void compare_basins(int largest[3], int size)
{
	int smallest;
	smallest = largest[0] < largest[1] ? 0 : 1;
	smallest = largest[smallest] < largest[2] ? smallest : 2;

	if (size > largest[smallest])
		largest[smallest] = size;
}
