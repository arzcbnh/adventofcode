/* Advent of Code 2021 - Day 15 *
 * Written by Henry Peaurt	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "strmanip.h"
#include "memmanage.h"

/* Types */
typedef struct {
	int x;
	int y;
	int risk;
	int total_risk;
	bool visited;
} node;

typedef struct {
	node map[500][500];
	int size; /* The only purpose of this variable is so the program works with the test */
} cntxt;

/* Function declarations */
void input_map(cntxt* c);
void expand_map(cntxt* c);
void copy_right(cntxt* c, int x, int y);
void copy_down(cntxt* c, int new_x, int y);
void init_nodes(cntxt* c);
void pathfind(cntxt* c);
node* get_shortest(cntxt* c);

int
main(void)
{
	cntxt c = {
		.map = {{{ 0 }}},
		.size = 0
	};

	input_map(&c);
	expand_map(&c);
	init_nodes(&c);
	pathfind(&c);

	node *n = get_shortest(&c);

	printf("Part 1: %i\n", n->total_risk);
	
	c.size *= 5;
	init_nodes(&c);
	pathfind(&c);

	n = get_shortest(&c);

	printf("Part 2: %i\n", n->total_risk);

	/*
	for (int y = 0; y < c.size; ++y) {
		for (int x = 0; x < c.size; ++x)
			printf("%i", c.map[x][y].risk);

		putchar('\n');
	}
	putchar('\n');
	for (int y = 0; y < c.size; ++y) {
		for (int x = 0; x < c.size; ++x)
			printf("%i", c.map[x][y].visited);

		putchar('\n');
	}
	putchar('\n');
	for (int y = 0; y < c.size; ++y) {
		for (int x = 0; x < c.size; ++x)
			printf("%i", c.map[x][y].total_risk);

		putchar('\n');
	}
	putchar('\n');
	*/

	return 0;
}

void
input_map(cntxt* c)
{
	char* s = str_input();
	size_t len = strlen(s);
	int y = 0;

	while (s != NULL) {
		for (size_t x = 0; x < len; x++)
			c->map[x][y].risk = s[x] - '0';

		y++;
		s = str_input();
	}

	c->size = (int) len;
}

void
expand_map(cntxt* c)
{
	/* Some confusing math goes on with this function. It copies (x, y) to the the first line of the grid of the *
	 * expanded map. So (0x, 0y) is copied to itself, then (1x, 0y), then (2x, 0y)... For every copy in the      *
	 * line, it also copies for the entire column. So after (0x, 0y) is copied to itself, it actually copies to  *
	 * (0x, 1y), then (0x, 2y)... Only after the entire column is complete, it continues completing the line to  *
	 * (1x, 0y). The new risk level is always being calculated as the challenge asked (wrap around level 9).     *
	 * Every iteration is very similar so I chose to abstract them behind functions, although it's still complex */

	for (int y = 0; y < c->size; y++) {
		for (int x = 0; x < c->size; x++)
			copy_right(c, x, y);
	}
}

void
copy_right(cntxt* c, int x, int y)
{
	int risk = c->map[x][y].risk;

	for (int i = 0; i < 5; i++) {
		int new_x = x + c->size * i;

		c->map[new_x][y].risk = risk;
		copy_down(c, new_x, y);

		risk++;
		risk = risk == 10 ? 1 : risk;
	}
}

void
copy_down(cntxt* c, int new_x, int y)
{
	int risk = c->map[new_x][y].risk;

	for (int i = 0; i < 5; i++) {
		int new_y = y + c->size * i;

		c->map[new_x][new_y].risk = risk;

		risk++;
		risk = risk == 10 ? 1 : risk;
	}
}

void
init_nodes(cntxt* c)
{
	for (int y = 0; y < c->size; y++) {
		for (int x = 0; x < c->size; x++) {
			node* n = &(c->map[x][y]);

			n->x = x;
			n->y = y;
			n->total_risk = -1; /* Placeholder for infinity/unknown */
			n->visited = false;
		}
	}

	c->map[0][0].total_risk = 0;
}

void
pathfind(cntxt* c)
{
	node *n = &(c->map[0][0]);
	int sz = c->size - 1; /* c.size stores the actual size, not the max index, so you have to sub 1 */

	while (n->x != sz || n->y != sz) {
		int x = n->x;
		int y = n->y;

		n->visited = true;

		node* neighbours[4] = {
			(x > 0 ? &(c->map[x - 1][y]) : NULL),
			(y > 0 ? &(c->map[x][y - 1]) : NULL),
			(x < sz ? &(c->map[x + 1][y]) : NULL),
			(y < sz ? &(c->map[x][y + 1]) : NULL)
		};

		for (int i = 0; i < 4; i++) {
			node *ngb = neighbours[i];

			if (ngb == NULL || ngb->visited)
				continue;

			int tmp = ngb->risk + n->total_risk;

			if (tmp < ngb->total_risk || ngb->total_risk == -1)
				ngb->total_risk = tmp;
		}						

		n = get_shortest(c);
	}
}

node*
get_shortest(cntxt* c)
{
	node* n = NULL;

	for (int y = 0; y < c->size; y++) {
		for (int x = 0; x < c->size; x++) {
			node *cmp = &(c->map[x][y]);

			if (
				cmp->total_risk > 0 && cmp->visited == false &&
				(n == NULL || n->total_risk > cmp->total_risk)
			   )
				n = cmp;
		}
	}

	return n;
}
