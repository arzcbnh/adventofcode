// Advent of Code 2021 - Day 05
// Written by Henry Peaurt

/* It's simple and  can use stack arrays here because the maps are predefined to be 1000x1000, but the structs inside
 * structs add a little more complexity */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strmanip.h"

// Types
typedef struct context {
	int map[1000][1000];
	struct vent {
		int x1, y1;
		int x2, y2;
		_Bool diagonal;
	} **vents;
	int vent_count;
} context;

// Function declarations
void get_vents(context *c);
void lay_vent(int map[1000][1000], struct vent *v);
int calc_overlaps(int map[1000][1000]);

int main(void)
{
	context c = { 0 };
	c.vents = NULL;
	c.vent_count = 0;

	get_vents(&c);

	for (int i = 0; i < c.vent_count; ++i) {
		if (!(c.vents[i]->diagonal))
			lay_vent(c.map, c.vents[i]);
	}
	
	printf("Part 1: %u\n", calc_overlaps(c.map));

	for (int i = 0; i < c.vent_count; ++i) {
		if (c.vents[i]->diagonal)
			lay_vent(c.map, c.vents[i]);
	}

	printf("Part 2: %u\n", calc_overlaps(c.map));

	for (int i = 0; i < c.vent_count; ++i)
		free(c.vents[i]);
	free(c.vents);

	return 0;
}

void get_vents(context *c)
{
	char *s = NULL;

	while ((s = str_input())) {
		int *coord = NULL;
		str_to_int_array(s, &coord);

		struct vent *v = malloc(sizeof(struct vent));
		v->x1 = coord[0];
		v->y1 = coord[1];
		v->x2 = coord[2];
		v->y2 = coord[3];
		v->diagonal = !(v->x1 == v->x2 || v->y1 == v->y2);

		c->vents = realloc(c->vents, sizeof(struct vent*) * (c->vent_count + 1));
		c->vents[c->vent_count++] = v;

		free(coord);
	}
}

void lay_vent(int map[1000][1000], struct vent *v)
{
	int x1 = v->x1;
	int x2 = v->x2;
	int y1 = v->y1;
	int y2 = v->y2;
	int opx = x1 <= x2 ? (x1 < x2 ? 1 : 0) : -1;
	int opy = y1 <= y2 ? (y1 < y2 ? 1 : 0) : -1;

	while (x1 != x2 || y1 != y2) {
		++map[y1][x1];
		x1 += opx;
		y1 += opy;
	}
	++map[y1][x1]; // The loop doesn't run for the last coordinate
}

int calc_overlaps(int map[1000][1000])
{
	int count = 0;

	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j)
			if (map[i][j] > 1) ++count;
	}

	return count;
}
