/* Advent of Code 2021 - Day 15 *
 * Written by Henry Peaurt	*/

/* I had previously got stuck on this program, and took around a month-long break from AoC because of it. It took me *
 * quit some time to understand Djikstra's algorithm, and although it's horribly inefficient, I'm very proud of      *
 * learning it and I'm grateful that AoC gave me this opportunity. I drafted a second, even more inefficient program *
 * that produces jpg images for each state of the pathfinding, but it was so bad and so slow that it crashed 4 nodes *
 * before completion, after running for an hour and a half. I decided to upgrade the program by learning the A*      *
 * algorithm and it turned out amazing!										     */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "strmanip.h"

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
	node* queue[250000];
	int size; /* The only purpose of this variable is so the program works with the test */
} cntxt;

/* Function declarations */
void input_map(cntxt* c);
void expand_map(cntxt* c);
void copy_right(cntxt* c, int x, int y);
void copy_down(cntxt* c, int new_x, int y);
void init_nodes(cntxt* c);
void pathfind(cntxt* c, node* n);
void update_neighbour(node *n, node* ngb);
void update_queue(cntxt* c, node* n);
int find_old_index(cntxt* c, node* n);
int find_new_index(cntxt* c, node* n);

int
main(void)
{
	cntxt c = {
		.map = {{{ 0 }}},
		.queue = { 0 },
		.size = 0
	};

	input_map(&c);
	expand_map(&c);

	for (int i = 0; i < 2; ++i) {
		init_nodes(&c);
		c.queue[0] = &(c.map[0][0]);

		node* n = c.queue[0];

		/* c.size stores the actual dimension, not the max index, so you need to sub 1 */
		while (n->x != c.size - 1 || n->y != c.size - 1) {
			pathfind(&c, n);
			n = c.queue[0];
			printf("%i %i %i\n", n->x, n->y, n->total_risk);
			for (int i = 0; c.queue[i] != NULL; i++)
				printf("(%i,%i), ", c.queue[i]->x, c.queue[i]->y);
			putchar('\n');
		}

		printf("Part %i: %i\n", i + 1, n->total_risk);
		n = &(c.map[0][0]);
		c.size *= 5;
	}

	return 0;
}

void
input_map(cntxt* c)
{
	char* s = str_input();
	c->size = (int) strlen(s);
	int y = 0;

	while (s != NULL) {
		for (int x = 0; x < c->size; x++)
			c->map[x][y].risk = s[x] - '0';

		y++;
		s = str_input();
	}
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
pathfind(cntxt* c, node* n)
{
	int sz = c->size - 1;
	int x = n->x;
	int y = n->y;

	n->visited = true;
	for (int i = 0; c->queue[i] != NULL; i++)
		c->queue[i] = c->queue[i + 1];

	node* neighbours[4] = {
		(x > 0 ? &(c->map[x - 1][y]) : NULL),
		(y > 0 ? &(c->map[x][y - 1]) : NULL),
		(x < sz ? &(c->map[x + 1][y]) : NULL),
		(y < sz ? &(c->map[x][y + 1]) : NULL)
	};

	for (int i = 0; i < 4; ++i) {
		node* ngb = neighbours[i];

		if (ngb == NULL || ngb->visited)
			continue;

		update_neighbour(n, ngb);
		update_queue(c, ngb);
	}
}

void
update_neighbour(node* n, node* ngb)
{
	int tmp = ngb->risk + n->total_risk;

	if (tmp < ngb->total_risk || ngb->total_risk == -1)
		ngb->total_risk = tmp;
}

void
update_queue(cntxt* c, node* n)
{
	int old = find_old_index(c, n);
	int new = find_new_index(c, n);
	int factor = (old < new) - (old > new); 

	/* Kinda confusing, but it goes up or down depending on the position of the old and new indexes */
	for (int i = old; i != new; i += factor)
		c->queue[i] = c->queue[i + factor];

	c->queue[new] = n;
}

int
find_old_index(cntxt* c, node* n)
{
	int i = 0;

	for (; c->queue[i] != NULL; i++) {
		if (c->queue[i]->x == n->x && c->queue[i]->y == n->y)
			break;
	}

	return i;
}

int
find_new_index(cntxt* c, node* n)
{
	int i = 0;

	for (; c->queue[i] != NULL; i++) {
		if (c->queue[i]->total_risk >= n->total_risk)
			break;
	}

	return i;
}
