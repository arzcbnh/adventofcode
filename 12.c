/* Advent of Code 2021 - Day 12 */
/* Written by Henry Peaurt */

/* The new version of this one turned out way better. Miles better. I'm also trying out a new style... */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "strmanip.h"
#include "memmanage.h"

/* Types */
typedef struct cave {
	char *name;
	bool is_small;
	int visits;

	struct cave **conn_list;
	int conn_cnt;
} cave;

typedef struct {
	cave **cave_list;
	int cave_cnt;

	cave **vstd_list;
	int vstd_cnt;
} cntxt;

/* Function declarations */
void input_caves(cntxt *c);
cave* get_cave(cntxt *c, char *name);
cave* alloc_cave(cntxt *c, char *name);
void connect_caves(cave *a, cave *b);
unsigned int pathfind(cave *curr, int max_vst, _Bool vstd_twice);

int
main(void)
{
	cntxt c = {
		.cave_list = NULL,
		.cave_cnt = 0,
		.vstd_list = NULL,
		.vstd_cnt = 0
	};

	input_caves(&c);

	cave *start = get_cave(&c, "start");

	printf("Part 1: %u\n", pathfind(start, 1, false));

	printf("Part 2: %u\n", pathfind(start, 2, false));

	return 0;
}

void
input_caves(cntxt *c)
{
	char *s = NULL;

	while ((s = str_input()))
	{
		char *name_a = str_word(0, s);
		char *name_b = str_word(1, s);

		cave *a = get_cave(c, name_a);
		if (a == NULL) a = alloc_cave(c, name_a);

		cave *b = get_cave(c, name_b);
		if (b == NULL) b = alloc_cave(c, name_b);

		connect_caves(a, b);
	}
}

cave*
get_cave(cntxt *c, char *name)
{
	for (int i = 0; i < c->cave_cnt; ++i) {
		if (strcmp(name, c->cave_list[i]->name) == 0)
			return c->cave_list[i];
	}

	return NULL;
}

cave*
alloc_cave(cntxt *c, char *name)
{
	cave *p = mem_alloc(sizeof(cave));
	p->name = name;
	p->is_small = 'a' <= *name && *name <= 'z';
	p->conn_list = NULL;
	p->conn_cnt = 0;

	c->cave_list = mem_realloc(c->cave_list, (c->cave_cnt + 1) * sizeof(cave*));
	c->cave_list[c->cave_cnt++] = p;

	return p;
}

void
connect_caves(cave *a, cave *b)
{
	a->conn_list = mem_realloc(a->conn_list, (a->conn_cnt + 1) * sizeof(cave*));
	b->conn_list = mem_realloc(b->conn_list, (b->conn_cnt + 1) * sizeof(cave*));

	a->conn_list[a->conn_cnt++] = b;
	b->conn_list[b->conn_cnt++] = a;
}

unsigned int
pathfind(cave *curr, int max_vst, _Bool vstd_twice)
{
	if (strcmp(curr->name, "end") == 0)
		return 1;

	if (
		curr->is_small && curr->visits >= 1 &&
		(strcmp(curr->name, "start") == 0 || max_vst == 1 || vstd_twice)
	   )
		return 0;

	++curr->visits;
	vstd_twice = vstd_twice || (curr->is_small && max_vst == 2 && curr->visits >= 2);

	unsigned int found_paths = 0;

	for (int i = 0; i < curr->conn_cnt; ++i) {
		found_paths += pathfind(curr->conn_list[i], max_vst, vstd_twice);
	}

	--curr->visits; /* The number of visits doesn't increase past 2, so it subs 1 after the twice-visited cave is
			 * processed and backtracks to tell the next paths the cave hasn't been visited twice */

	return found_paths;
}
