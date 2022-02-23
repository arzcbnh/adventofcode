/* Advent of Code 2021 - Day 13 *
 * Written by Henry Peaurt	*/

/* I could've merged the input functions into one, but I thought it looked too complex so I kept them separate */

#include <stdio.h>
#include <stdbool.h>

#include "strmanip.h"
#include "memmanage.h"

/* Types */
typedef struct {
	int x;
	int y;
} point;

typedef struct {
	char axis;
	int n;
} order;

typedef struct {
	bool **paper;
	int x;
	int y;

	point **pt_list;
	int pt_cnt;

	order **ord_list;
	int ord_cnt;
} cntxt;

/* Function declarations */
void input_points(cntxt *c);
void input_orders(cntxt *c);
void make_paper(cntxt *c);
void fold_paper(cntxt *c, order *ord);
void resize_paper(cntxt *c, order *ord);
int count_dots(cntxt *c);
void print_paper(cntxt *c);
/*
int createPoints(point ***points, int *x, int *y);
int getCoordinate(point *coordinate);
void copyDots(point **points, int point_amount, bool **dots, int x);
int createFolds(fold ***folds);
int getInstruction(fold *instruction);
void foldPaper(bool **dots, int x, int y, char axis, int n);
void resizePaper(bool **dots, int x, int y, char axis, int n);
int countDots(bool *dots, int dot_amount);
*/

int
main(void)
{
	cntxt c = {
		.paper = NULL,
		.x = 0,
		.y = 0,
		.pt_list = 0,
		.pt_cnt = 0,
		.ord_list = NULL,
		.ord_cnt = 0
	};

	input_points(&c);
	input_orders(&c);
	make_paper(&c);

	fold_paper(&c, c.ord_list[0]);
	resize_paper(&c, c.ord_list[0]);
	printf("Part 1: %u\n", count_dots(&c));

	for (int i = 1; i < c.ord_cnt; ++i) {
		fold_paper(&c, c.ord_list[i]);
		resize_paper(&c, c.ord_list[i]);
	}

	printf("Part 2:\n");
	print_paper(&c);

	mem_clean();

	return 0;
}

void
input_points(cntxt *c)
{
	point **pts = NULL;
	int cnt = 0;
	int cap = 0;
	char *s = NULL;

	while ((s = str_input())) {
		int p1 = atoi(str_word(0, s));
		int p2 = atoi(str_word(1, s));

		if (cnt == cap)
			pts = mem_realloc(pts, (cap += 100) * sizeof(point*));

		point *p = mem_alloc(sizeof(point));
		p->x = p1;
		p->y = p2;

		c->x = c->x > p->x ? c->x : p->x;
		c->y = c->y > p->y ? c->y : p->y;

		pts[cnt++] = p;
	}

	c->pt_list = pts;
	c->pt_cnt = cnt;
	++c->x;
	++c->y;
}

void
input_orders(cntxt *c)
{
	order **ords = NULL;
	int cnt = 0;
	int cap = 0;
	char *s = NULL;

	while ((s = str_input())) {
		char axis = str_word(2, s)[0];
		int n = atoi(str_word(3, s));

		if (cnt == cap)
			ords = mem_realloc(ords, (cap += 100) * sizeof(order*));

		order *o = mem_alloc(sizeof(order));
		o->axis = axis;
		o->n = n;

		ords[cnt++] = o;
	}

	c->ord_list = ords;
	c->ord_cnt = cnt;
}

void
make_paper(cntxt *c)
{
	bool **paper = mem_alloc(c->x * sizeof(bool*));

	for (int x = 0; x < c->x; ++x)
		paper[x] = mem_alloc(c->y * sizeof(bool));

	for (int i = 0; i < c->pt_cnt; ++i) {
		int x = c->pt_list[i]->x;
		int y = c->pt_list[i]->y;

		(paper[x])[y] = true;
	}

	c->paper = paper;
}

void
fold_paper(cntxt *c, order *ord)
{
	bool **paper = c->paper;
	char axis = ord->axis;
	int n = ord->n;

	for (int y = 0; y < c->y; ++y) {
		for (int x = 0; x < c->x; ++x) {
			bool oppst = false;

			if (x < n && axis == 'x')
				oppst = (paper[n + n - x])[y];
			else if (y < n && axis == 'y')
				oppst = (paper[x])[n + n - y];
			else
				break;

			(paper[x])[y] |= oppst;
		}
	}
}

void
resize_paper(cntxt *c, order *ord)
{
	if (ord->axis == 'x') {
		c->x -= ord->n + 1;
	} else {
		c->y -= ord->n + 1;
	}
}

int
count_dots(cntxt *c)
{
	int cnt = 0;

	for (int y = 0; y < c->y; ++y) {
		for (int x = 0; x < c->x; ++x)
			cnt += (c->paper[x])[y];
	}

	return cnt;
}

void
print_paper(cntxt *c)
{
	for (int y = 0; y < c->y; ++y) {
		for (int x = 0; x < c->x; ++x) {
			putchar((c->paper[x])[y] ? '#' : '.');
		}

	putchar('\n');
	}
}
