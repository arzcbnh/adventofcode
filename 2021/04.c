// Advent of Code 2021 - Day 04
// Written by Henry Peaurt

/* Long, but simple */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strmanip.h"

// Types
typedef struct context {
	int *draws;
	int draw_count;

	int **tables;
	int table_count;

	int **won_tables;
	int won_count;
} context;

// Function declarations
void load_tables(context *c);
void mark_table(int n, int *tab);
_Bool check_win(int *tab);
void move_won_table(context *c, int i, int wn);
unsigned int calc_score(int* tab);

int main(void)
{
	context c;
	c.draws = NULL;
	c.tables = c.won_tables = NULL;
	c.draw_count = c.table_count = c.won_count = 0;

	load_tables(&c);

	for (int i = 0; i < c.draw_count; ++i) {
		for (int j = 0; j < c.table_count; ++j) {
			mark_table(c.draws[i], c.tables[j]);
			_Bool won = check_win(c.tables[j]);

			if (won) move_won_table(&c, j--, c.draws[i]);
		}
	}

	printf("Part 1: %u\n", calc_score(c.won_tables[0]));
	printf("Part 2: %u\n", calc_score(c.won_tables[c.won_count - 1]));

	free(c.draws);
	for (int i = 0; i < c.table_count; ++i)
		free(c.tables[i]);
	free(c.tables);
	for (int i = 0; i < c.won_count; ++i)
		free(c.won_tables[i]);
	free(c.won_tables);

	return 0;
}

void load_tables(context *c)
{
	char *s = str_input();
	c->draw_count = str_to_int_array(s, &c->draws);

	// Every loop is a table. The first str_input() reads the empty line before the table
	while ((s = str_input())) {
		for (int i = 0; i < 5; ++i)
			s = str_push(str_append(s, str_input()), ' '); // Blank char to separate number strings

		int *t = NULL;
		str_to_int_array(s, &t);
		c->tables = realloc(c->tables, sizeof(int*) * (c->table_count + 1));
		c->tables[c->table_count++] = t;
	}
}

void mark_table(int n, int *tab)
{
	for (int i = 0; i < 25; ++i) {
		if (tab[i] == n)
			tab[i] -= 100; // Bingo is below 100, so it's "cryptographed" to n-100 to mark it
	}
}

_Bool check_win(int *tab)
{
	for (int i = 0; i < 5; ++i) {
		int row = 0;
		int col = 0;

		for (int j = 0; j < 5; ++j) {
			if (tab[i * 5 + j] < 0) row += 1;
			if (tab[i + 5 * j] < 0) col += 1;
		}

		if (row == 5 || col == 5)
			return true;
	}
	
	return false;
}

void move_won_table(context *c, int i, int wn)
{
	(c->tables[i])[25] = wn;

	c->won_tables = realloc(c->won_tables, sizeof(int*) * (c->won_count + 1));
	c->won_tables[c->won_count++] = c->tables[i];
	c->tables[i] = c->tables[--c->table_count];
	c->tables[c->table_count] = NULL;
}

unsigned int calc_score(int* tab)
{
	unsigned int score = 0;

	for (int i = 0; i < 25; ++i)
		score += tab[i] >= 0 ? tab[i] : 0;

	return score * tab[25];
}
