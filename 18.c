// Advent of Code 2021 - Day 18
// Written by Henry Peaurt

// It was hard to implement the binary tree because it kept causing bugs. I wonder if manipulating strings directly
// would've been easier. The side duality of the binary tree led to convoluted and repetitive long functions for
// managing each side, but merging these functions reduced the program by more than 100 lines.

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "strmanip.h"
#include "memmanage.h"

// Enums
typedef enum {
	NOTHING,
	EXPLODED,
	SPLIT
} operation_t;

typedef enum {
	NONE,
	LEFT,
	RIGHT
} side_t;

// Structs
typedef struct pair {
	int n_left;
	int n_right;
	struct pair* p_left;
	struct pair* p_right;
	struct pair* parent;
} pair;

typedef struct {
	pair* address;
	side_t side;
} close_pair;

typedef struct {
	pair** pair_list;
	int pair_cnt;

	pair* sum;
} cntxt;

// Function declarations
void input_pairs(cntxt* c);
pair* decode_pair(char* s, pair* parent);
pair* alloc_pair(int n_l, int n_r, pair* p_l, pair* p_r, pair* parent);
void sum_all(cntxt* c);
pair* sum_pairs(pair* a, pair* b);
pair* copy_pair(pair* p, pair* parent);
void reduce_pair(pair* p);
operation_t check_explosion(pair* p, int depth);
operation_t check_split(pair *p);
void explode_pair(pair* p);
close_pair get_closest_number(pair* p, side_t side);
void split_pair(pair* p, side_t side);
long calc_magnitude(pair* p);
long calc_max_sum_two(cntxt* c);

int
main(void)
{
	cntxt c = {
		.pair_list = NULL,
		.pair_cnt = 0,
		.sum = NULL
	};

	input_pairs(&c);
	sum_all(&c);

	printf("Part 1: %lu\n", calc_magnitude(c.sum));
	printf("Part 2: %lu\n", calc_max_sum_two(&c));

	return 0;
}

void
input_pairs(cntxt* c)
{
	pair** list = mem_alloc(128 * sizeof(pair*));
	int cnt = 0;
	int cap = 128;
	char* s = str_input();

	while (s != NULL) {
		if (cnt == cap) {
			cap += 128;
			list = mem_realloc(list, cap * sizeof(pair*));
		}

		pair* p = decode_pair(s, NULL);
		list[cnt] = p;
		cnt++;

		s = str_input();
	}

	c->pair_list = list;
	c->pair_cnt = cnt;
}

pair*
decode_pair(char* s, pair* parent)
{
	static int i;
	pair* p = alloc_pair(0, 0, NULL, NULL, parent);

	i = p->parent == NULL ? 1 : i + 1;

	if (s[i] == '[') {
		p->p_left = decode_pair(s, p);
	} else {
		p->n_left = s[i] - '0';
	}

	i += 2;

	if (s[i] == '[') {
		p->p_right = decode_pair(s, p);
	} else {
		p->n_right = s[i] - '0';
	}

	i++;

	return p;
}

pair*
alloc_pair(int n_l, int n_r, pair* p_l, pair* p_r, pair* parent)
{
	pair* p = mem_alloc(sizeof(pair));

	p->n_left = n_l;
	p->n_right = n_r;
	p->p_left = p_l;
	p->p_right = p_r;
	p->parent = parent;

	return p;
}

void
sum_all(cntxt* c)
{
	c->sum = c->pair_list[0];

	for (int i = 1; i < c->pair_cnt; i++) {
		c->sum = sum_pairs(c->sum, c->pair_list[i]);
		reduce_pair(c->sum);
	}
}

pair*
sum_pairs(pair* a, pair* b)
{
	pair* p = alloc_pair(0, 0, NULL, NULL, NULL);

	p->p_left = copy_pair(a, p);
	p->p_right = copy_pair(b, p);

	return p;
}

pair*
copy_pair(pair* p, pair* parent)
{
	pair* c = alloc_pair(0, 0, NULL, NULL, parent);
	c->parent = parent;

	if (p->p_left == NULL) {
		c->n_left = p->n_left;
	} else {
		c->p_left = copy_pair(p->p_left, c);
	}

	if (p->p_right == NULL) {
		c->n_right = p->n_right;
	} else {
		c->p_right = copy_pair(p->p_right, c);
	}

	return c;
}

void
reduce_pair(pair* p)
{
start: // It has to always follow this order every operation, so I figured a goto would make it very compact and clear
	if (check_explosion(p, 1) == EXPLODED) {
		goto start;
	}

	if (check_split(p) == SPLIT) {
		goto start;
	}
}

operation_t
check_explosion(pair* p, int depth)
{
	if (p->p_left != NULL && check_explosion(p->p_left, depth + 1) == EXPLODED) {
		return EXPLODED;
	}

	if (p->p_left == NULL && p->p_right == NULL && depth > 4) {
		explode_pair(p);
		return EXPLODED;
	}

	if (p->p_right != NULL && check_explosion(p->p_right, depth + 1) == EXPLODED) {
		return EXPLODED;
	}

	return NOTHING;
}

operation_t
check_split(pair *p)
{
	if (p->p_left != NULL && check_split(p->p_left) == SPLIT) {
		return SPLIT;
	}

	if (p->p_left == NULL && p->n_left >= 10) {
		split_pair(p, LEFT);
		return SPLIT;
	}

	if (p->p_right == NULL && p->n_right >= 10) {
		split_pair(p, RIGHT);
		return SPLIT;
	}

	if (p->p_right != NULL && check_split(p->p_right) == SPLIT) {
		return SPLIT;
	}

	return NOTHING;
}

void
explode_pair(pair* p)
{
	close_pair c = get_closest_number(p, LEFT);

	if (c.side == LEFT) {
		c.address->n_left += p->n_left;
	} else if (c.side == RIGHT) {
		c.address->n_right += p->n_left;
	}

	c = get_closest_number(p, RIGHT);

	if (c.side == LEFT) {
		c.address->n_left += p->n_right;
	} else if (c.side == RIGHT) {
		c.address->n_right += p->n_right;
	}

	pair* parent = p->parent;

	if (parent->p_left == p) {
		parent->p_left = NULL;
		parent->n_left = 0;
	} else if (parent->p_right == p) {
		parent->p_right = NULL;
		parent->n_right = 0;
	}
}

close_pair
get_closest_number(pair* p, side_t side)
{
	pair* prev = p;
	p = p->parent;

	// This goes up the tree
	while (
		(side == LEFT && p->p_left == prev) ||
		(side == RIGHT && p->p_right == prev)
	      ) {
		if (p->parent == NULL) {
			close_pair c = { .address = NULL, .side = NONE };
			return c;
		} else {
			prev = p;
			p = p->parent;
		}
	}

	if (
		(side == LEFT && p->p_left == NULL) ||
		(side == RIGHT && p->p_right == NULL)
	   ) {
		close_pair c = { .address = p, .side = side == LEFT ? LEFT : RIGHT };
		return c;
	}

	p = side == LEFT ? p->p_left : p->p_right;

	// This goes down the tree. There's a side inversion factor going on that might be confusing.
	while (
		(side == LEFT && p->p_right != NULL) ||
		(side == RIGHT && p->p_left != NULL)
	      ) {
		p = side == LEFT ? p->p_right : p->p_left;
	}

	close_pair c = { .address = p, .side = side == LEFT ? RIGHT : LEFT };
	return c;
}

void
split_pair(pair* p, side_t side)
{
	if (side == LEFT) {
		int n = p->n_left;
		p->p_left = alloc_pair(n / 2.0, n / 2.0 + 0.5, NULL, NULL, p);
	} else { // side == RIGHT
		int n = p->n_right;
		p->p_right = alloc_pair(n / 2.0, n / 2.0 + 0.5, NULL, NULL, p);
	}
}

long
calc_magnitude(pair* p)
{
	long left = p->p_left == NULL ? p->n_left : calc_magnitude(p->p_left);
	long right = p->p_right == NULL ? p->n_right : calc_magnitude(p->p_right);

	return 3 * left + 2 * right;
}

long
calc_max_sum_two(cntxt* c)
{
	long max_sum = 0;

	for (int i = 0; i < c->pair_cnt; i++) {
		for (int j = 0; j < c->pair_cnt; j++) {
			c->sum = sum_pairs(c->pair_list[i], c->pair_list[j]);
			reduce_pair(c->sum);

			long tmp = calc_magnitude(c->sum);
			max_sum = max_sum > tmp ? max_sum : tmp;
		}
	}

	return max_sum;
}
