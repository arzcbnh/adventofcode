// Advent of Code 2021 - Day 08
// Written by Henry Peaurt

/* The program turned out very long and complex, with some quite arbitrary algorithms. I regret not using better and
 * more descriptive variable names, maybe I should've used arrays for the digit segment storage... */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmanip.h"

// Constants
const char *SEGS_FOR[] = { "abcefg",	// 0
			   "cf",	// 1
			   "acdeg",	// 2
			   "acdfg",	// 3
			   "bcdf",	// 4
			   "abdfg",	// 5
			   "abdefg",	// 6
			   "acf",	// 7
			   "abcdefg",	// 8
			   "abcdfg"	// 9
};
enum segments { A, B, C, D, E, F, G };

// Types
typedef struct display {
	char *s;
	char *segs_for[10];
	char key[8];
} display;

typedef struct context {
	display **d_list;
	int d_count;

	int *outputs;
} context;

// Function declarations
void get_displays(context *c);
void find_special_digits(display *d);
void find_digit(int n, int k, display *d);
int calc_matches(char* x, char* y);
char find_key(char *in, char *from);
void store_output(display *d, int *n);

int main(void)
{
	context c = { 
		.d_list = NULL,
		.d_count = 0,
		.outputs = NULL
	};

	get_displays(&c);

	c.outputs = malloc(sizeof(int) * c.d_count);

	for (int i = 0; i < c.d_count; ++i) {
		display *d = c.d_list[i];

		find_special_digits(d);

		// Digits can be found when this order of searching is followed
		find_digit(3, 1, d);
		find_digit(6, 1, d);
		find_digit(9, 3, d);
		find_digit(2, 9, d);
		find_digit(5, 2, d);
		find_digit(0, 5, d);

		d->key[A] = find_key(d->segs_for[7], d->segs_for[1]);
		d->key[B] = find_key(d->segs_for[4], d->segs_for[3]);
		d->key[C] = find_key(d->segs_for[3], d->segs_for[5]);
		d->key[D] = find_key(d->segs_for[8], d->segs_for[0]);
		d->key[E] = find_key(d->segs_for[8], d->segs_for[9]);
		d->key[F] = find_key(d->segs_for[1], d->segs_for[2]);

		// Key G needs a custom search pattern to be found
		char search_pattern[6] = { 0 };
		strcpy(search_pattern, d->segs_for[4]);
		search_pattern[4] = d->key[A];
		d->key[G] = find_key(d->segs_for[9], search_pattern);

		store_output(d, c.outputs + i);
	}

	long special_count = 0;
	long sum = 0;

	for (int i = 0; i < c.d_count; ++i) {
		sum += c.outputs[i];

		for (int j = 0; j < 4; ++j) {
			int n = c.outputs[i] % 10;

			if (n == 1 || n == 4 || n == 7 || n == 8)
				++special_count;

			c.outputs[i] /= 10;
		}
	}

	printf("Part 1: %lu\n", special_count);
	printf("Part 2: %lu\n", sum);

	for (int i = 0; i < c.d_count; ++i) {
		for (int j = 0; j < 10; ++j)
			free(c.d_list[i]->segs_for[j]);
		free(c.d_list[i]->s);
		free(c.d_list[i]);
	}
	free(c.d_list);
	free(c.outputs);

	return 0;
}

void get_displays(context *c)
{
	int cap = 0;
	char *s = NULL;

	while ((s = str_input())) {
		if (c->d_count == cap)
			c->d_list = realloc(c->d_list, sizeof(display*) * (cap += 100));

		display *d = malloc(sizeof(display));
		d->s = s;
		c->d_list[c->d_count++] = d;
	}
}

void find_special_digits(display *d)
{
	int found = 0;
	int i = 0;

	while (found < 4) {
		char *w = str_word(i++, d->s);

		switch (strlen(w)) {
		case 2:
			d->segs_for[1] = w;
			++found;
			continue;
		case 3:
			d->segs_for[7] = w;
			++found;
			continue;
		case 4:
			d->segs_for[4] = w;
			++found;
			continue;
		case 7:
			d->segs_for[8] = w;
			++found;
			continue;
		}

		free(w);
	}
}

void find_digit(int n, int k, display *d)
{
	// The segments of a number "n" can be found when it matches x times with digit "k"
	int i = 0;
	char *w = NULL;
	int exp_matches = calc_matches((char*)SEGS_FOR[n], (char*)SEGS_FOR[k]);

	do {
		do {
			free(w);
			w = str_word(i++, d->s);
		} while (strlen(w) != strlen(SEGS_FOR[n]));
	} while (exp_matches != calc_matches(d->segs_for[k], w));

	d->segs_for[n] = w;
}

int calc_matches(char* x, char* y)
{
	int len_x = strlen(x);
	int len_y = strlen(y);
	int matches = 0;

	for (int i = 0; i < len_x; ++i) {
		for (int j = 0; j < len_y; ++j) {
			if (x[i] == y[j])
				++matches;
		}
	}

	return matches;
}

char find_key(char *in, char *from)
{
	// The key stands out in digit "in" when compared against digit "from"
	int len_in = strlen(in);
	int len_from = strlen(from);

	for (int i = 0; i < len_in; ++i) {
		for (int j = 0; in[i] != from[j]; ++j) {
			if (j == len_from)
				return in[i];
		}
	}

	return '\0';
}

void store_output(display *d, int *n)
{
	*n = 0;

	// i starts at 11 so it skips the 10 digits and the | separator
	for (int i = 11; i < 15; ++i) {
		char *unknown = str_word(i, d->s);
		unsigned long len = strlen(unknown);

		for (int i = 0; i < 10; ++i) {
			char *compare = d->segs_for[i];

			// If find_key() doesn't find a different key, then it's the same number
			if (strlen(compare) == len && !(find_key(unknown, compare))) {
				*n = *n * 10 + i;
				break;
			}
		}

		free(unknown);
	}
}
