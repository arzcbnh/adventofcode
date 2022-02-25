// Advent of Code 2021 - Day 15
// Written by Henry Peaurt

// I had previously got stuck on this program, and took around a month-long break from AoC because of it. It took me
// quite some time to understand Dijkstra's algorithm, and although it's horribly inefficient, I'm very proud of
// learning it and I'm grateful that AoC gave me this opportunity. I drafted a second, even more inefficient program
// that produces jpg images for each state of the pathfinding, but it was so bad and so slow that it crashed 4 nodes
// before completion, after running for an hour and a half. I decided to upgrade the program by learning the A*
// algorithm and it turned out amazing! I mean, it looks horrible, but it works...

#include <stdio.h>
#include <stdbool.h>

#include "memmanage.h"

// Types
typedef struct {
	int x;
	int y;
	int risk;
	int total_risk;
	bool visited;
} node;

typedef struct {
	node map[500][500];
	int size;

	node** queue;
	int q_cap;
} cntxt;

// Function declarations
void input_map(cntxt* c);
void expand_map(cntxt* c);
void copy_right(cntxt* c, int x, int y);
void copy_down(cntxt* c, int new_x, int y);
void init_nodes(cntxt* c);
void pathfind(cntxt* c, node* n);
void update_queue(cntxt* c, node* n);
void pull_queue(cntxt* c);
void init_queue(cntxt* c);
void expand_queue(cntxt* c);

int
main(void)
{
	cntxt c = {
		.map = {{{ 0 }}},
		.size = 0,
		.queue = mem_alloc(512 * sizeof(node*)),
		.q_cap = 512
	};

	input_map(&c);

	/*
	for (int y = 0; y < c.size; ++y) {
		for (int x = 0; x < c.size; ++x)
			printf("%i", c.map[x][y].risk);

		putchar('\n');
	}
	putchar('\n');
	*/

	expand_map(&c);

	for (int i = 0; i < 2; ++i) {
		init_nodes(&c);
		init_queue(&c);
		c.queue[0] = &(c.map[0][0]);

		node* n = c.queue[0];

		/* c.size stores the actual dimension, not the max index, so you need to sub 1 */
		while (n->x != c.size - 1 || n->y != c.size - 1) {
			pathfind(&c, n);
			n = c.queue[0];
			/*
			printf("%i %i %i\n", n->x, n->y, n->total_risk);
			for (int i = 0; c.queue[i] != NULL; i++)
				printf("(%i,%i), ", c.queue[i]->x, c.queue[i]->y);
			putchar('\n');
			*/
		}

		printf("Part %i: %i\n", i + 1, n->total_risk);
		n = &(c.map[0][0]);
		c.size *= 5;
	}

	/*
	for (int y = 0; y < 100; ++y) {
		for (int x = 0; x < 100; ++x)
			if (c.map[99][99].path->)
			printf("\033[1m%i\033[0m", c.map[x][y].risk);
			else
			printf("%i", c.map[x][y].risk);

		putchar('\n');
	}
	putchar('\n');
	*/

	return 0;
}

void
input_map(cntxt* c)
{
	char a = getchar();
	int y = 0;

	while (a != EOF) {
		int x = 0;

		while (a != '\n') {
			c->map[x++][y].risk = a - '0';
			a = getchar();
		}

		y++;
		a = getchar();
	}

	c->size = y;
}

void
expand_map(cntxt* c)
{
	// Some confusing math goes on with this function. It copies (x, y) to the the first line of the grid of the
	// expanded map. So (0x, 0y) is copied to itself, then (1x, 0y), then (2x, 0y)... For every copy in the line,
	// it also copies for the entire column. So after (0x, 0y) is copied to itself, it actually copies to (0x, 1y),
	// then (0x, 2y)... Only after the entire column is complete, it continues completing the line to (1x, 0y). The
	// new risk level is always being calculated as the challenge asked (wrap around level 9). Every iteration is
	// very similar so I chose to abstract them behind functions, although it's still complex.

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
	int sz = c->size;

	for (int y = 0; y < sz; y++) {
		for (int x = 0; x < sz; x++) {
			node* n = &(c->map[x][y]);

			n->x = x;
			n->y = y;
			n->total_risk = -1; // Placeholder for infinity/unknown.
			n->visited = false;
		}
	}

	c->map[0][0].total_risk = 0;
}

void
pathfind(cntxt* c, node* n)
{
	int sz = c->size - 1; // c.size is storing the actual size, so to access the index, you have to sub 1
	int x = n->x;
	int y = n->y;

	n->visited = true;
	pull_queue(c);

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

		int tmp = ngb->risk + n->total_risk;

		if (tmp < ngb->total_risk || ngb->total_risk == -1)
			ngb->total_risk = tmp;

		/*
		update_path(n, ngb);
		*/
		update_queue(c, ngb);
	}
}

/*
void
update_path(node* n, node* ngb)
{
	int i = 0;

	for (i = 0; n->path[i] != NULL; n++)
		t->path[i]=f->path[i];

	f->path[i] = NULL;
}
*/

void
update_queue(cntxt* c, node* n)
{
	expand_queue(c);

	node** q = c->queue;
	int old = 0;
	int new = 0;

	for (; q[old] != NULL; old++) {
		if (q[old]->x == n->x && q[old]->y == n->y)
			break;
	}

	for (; q[new] != NULL; new++) {
		// Calculate the distance to the final node and sum the total risk
		int dist_q = q[new]->total_risk + (c->size * 2) - q[new]->x - q[new]->y - 2;
		int dist_n = n->total_risk 	+ (c->size * 2) - n->x 	    - n->y	- 2;

		if (dist_q >= dist_n)
			break;
	}

	// Kinda confusing, but it goes up or down depending on the position of the old and new indexes.
	int factor = (old < new) - (old > new); 

	for (int i = old; i != new; i += factor)
		q[i] = q[i + factor];

	q[new] = n;
}

void
pull_queue(cntxt* c)
{
	// If the capacity is full and it pulls, it'll access memory after the allocated block, so it needs to check
	// for capacity - 1.

	for (int i = 0; c->queue[i] != NULL || i == c->q_cap - 1; i++)
		c->queue[i] = c->queue[i + 1];

	c->queue[c->q_cap - 1] = NULL;
}

void
init_queue(cntxt* c)
{
	for (int i = 0; c->queue[i] != NULL || i == c->q_cap; i++)
		c->queue[i] = NULL;
}

void
expand_queue(cntxt* c)
{
	if (c->queue[c->q_cap - 1] != NULL) {
		c->q_cap += 512;
		c->queue = mem_realloc(c->queue, c->q_cap * sizeof(node*));
		for (int i = c->q_cap - 512; i < c->q_cap; i++)
			c->queue[i] = NULL;
	}
}
