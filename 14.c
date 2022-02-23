/* Advent of Code 2021 - Day 14 *
 * Written by Henry Peaurt	*/

/* Not sure if it was a good idea to typedef the struct as "rule" and name the variables as "rule", but shortening   *
 * to "r" seemed worse. I'm trying to break down functions and keep them from "nesting" more.			     */

#include <stdio.h>
#include <string.h>

#include "strmanip.h"
#include "memmanage.h"

/* Types */
typedef struct{
	char pair[2];
	char result;
	long old_cnt;
	long new_cnt;
} rule;

typedef struct {
	char name;
	long cnt;
} atom;

typedef struct {
	rule **rule_list;
	int rule_cnt;

	char edge[2];

	atom **atom_list;
	int atom_cnt;
} cntxt;

/* Function declarations */
void input_rules(cntxt *c);
void read_polymer(cntxt *c, char *polymer);
rule* get_rule(cntxt *c, char pair[2]);
void step_polymer(cntxt *c);
void record_atoms(cntxt *c);
atom* get_atom(cntxt *c, char name);
void alloc_atom(cntxt *c, char name);
void calc_atoms(cntxt *c);
long sub_more_less_common(cntxt *c);

int
main(void)
{
	cntxt c = {
		.rule_list = NULL,
		.rule_cnt = 0,
		.edge = { 0 },
		.atom_list = NULL,
		.atom_cnt = 0
	};
	char *polymer = str_input();

	input_rules(&c);
	read_polymer(&c, polymer);
	record_atoms(&c);

	for (int i = 0; i < 40; ++i) {
		if (i == 10) {
			calc_atoms(&c);
			printf("Part 1: %lu\n", sub_more_less_common(&c));
		}

		step_polymer(&c);
	}

	calc_atoms(&c);
	printf("Part 2: %lu\n", sub_more_less_common(&c));

	mem_clean();

	return 0;
}

void
input_rules(cntxt *c)
{
	char *s = str_input(); /* Skip newline after the polymer */
	int cap = 0;

	while ((s = str_input())) {
		if (c->rule_cnt == cap)
			c->rule_list = mem_realloc(c->rule_list, (cap += 128) * sizeof(rule*));

		rule *r = mem_alloc(sizeof(rule));
		r->pair[0] = s[0];
		r->pair[1] = s[1];
		r->result = s[6];
		r->old_cnt = 0;
		r->new_cnt = 0;

		c->rule_list[c->rule_cnt++] = r;
	}
}

void
read_polymer(cntxt *c, char *polymer)
{
	size_t len = strlen(polymer) - 1; /* More like the index rather than the length */

	for (size_t i = 0; i < len; ++i) {
		char pair[2] = { polymer[i], polymer[i + 1] };
		rule *r = get_rule(c, pair);

		++r->old_cnt;
	}

	c->edge[0] = polymer[0];
	c->edge[1] = polymer[len];
}

rule*
get_rule(cntxt *c, char pair[2])
{
	for (int i = 0; i < c->rule_cnt; ++i) {
		char *l_pair = c->rule_list[i]->pair;

		if (pair[0] == l_pair[0] && pair[1] == l_pair[1])
			return c->rule_list[i];
	}

	return NULL;
}

void
step_polymer(cntxt *c)
{
	rule **list = c->rule_list;

	for (int i = 0; i < c->rule_cnt; ++i) {
		rule *r = list[i];
		rule *p = NULL;
		char result[2] = { 0 };

		result[0] = r->pair[0];
		result[1] = r->result;
		p = get_rule(c, result);
		p->new_cnt += r->old_cnt;

		result[0] = r->result;
		result[1] = r->pair[1];
		p = get_rule(c, result);
		p->new_cnt += r->old_cnt;
	}

	for (int i = 0; i < c->rule_cnt; ++i) {
		rule *p = list[i];
		p->old_cnt = p->new_cnt;
		p->new_cnt = 0;
	}
}

void
record_atoms(cntxt *c)
{
	for (int i = 0; i < c->rule_cnt; ++i) {
		rule *r = c->rule_list[i];

		for (int j = 0; j < 2; ++j) {
			if (get_atom(c, r->pair[j]) == NULL)
				alloc_atom(c, r->pair[j]);
		}
	}
}

atom*
get_atom(cntxt *c, char name)
{
	for (int i = 0; i < c->atom_cnt; ++i) {
		if (name == c->atom_list[i]->name)
			return c->atom_list[i];
	}

	return NULL;
}

void
alloc_atom(cntxt *c, char name)
{
	static int cap;

	if (c->atom_cnt == cap)
		c->atom_list = mem_realloc(c->atom_list, (cap += 64) * sizeof(atom*));

	atom *a = mem_alloc(sizeof(atom));
	a->name = name;
	a->cnt = 0;

	c->atom_list[c->atom_cnt++] = a;
}

void
calc_atoms(cntxt *c)
{
	for (int i = 0; i < c->rule_cnt; ++i) {
		rule *r = c->rule_list[i];
		atom *a = NULL;

		a = get_atom(c, r->pair[0]);
		a->cnt += r->old_cnt;

		a = get_atom(c, r->pair[1]);
		a->cnt += r->old_cnt;
	}

	for (int i = 0; i < c->atom_cnt; ++i) {
		atom *a = c->atom_list[i];
		a->cnt /= 2;

		if (a->name == c->edge[0] || a->name == c->edge[1])
			++a->cnt;
	}
}

long
sub_more_less_common(cntxt *c)
{
	long less = c->atom_list[0]->cnt;
	long more = c->atom_list[0]->cnt;

	for (int i = 1; i < c->atom_cnt; ++i) {
		long cnt = c->atom_list[i]->cnt;

		less = less < cnt ? less : cnt;
		more = more > cnt ? more : cnt;

		c->atom_list[i]->cnt = 0; /* This is so the atoms can be recounted later */
	}

	return more - less;
}
