// Advend of Code - Day 24
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "strmanip.h"
#include "memmanage.h"

// Types
typedef struct { int i1, i2, n; } OpData;

// Function declarations
void input_data(OpData *d);
void skip_lines(int n);
long find_model(OpData d[], bool part1);
long invert_long(long n);

int
main(void)
{
	OpData d[7];
	input_data(d);

	printf("Part 1: %lu\n", find_model(d, true));
	printf("Part 2: %lu\n", find_model(d, false));

	mem_clean();

	return 0;
}

void
input_data(OpData *d)
{
	struct { int w, c; } pair[7];
	int i = 0;

	for (int w = 0; w < 14; w++) {
		skip_lines(4);
		
		int a = atoi(str_word(2, str_input()));
		int b = atoi(str_word(2, str_input()));

		skip_lines(9);

		int c = atoi(str_word(2, str_input()));

		skip_lines(2);

		if (a == 1) {
			pair[i].w = w;
			pair[i++].c = c;
		} else {
			d->i1 = pair[--i].w;
			d->i2 = w;
			(d++)->n = pair[i].c + b;
		}
	}
}

void
skip_lines(int n)
{
	for (int ln = 0; ln < n; ln += getchar() == '\n')
		;
}

long
find_model(OpData d[], bool part1)
{
	long model = 0;

	for (int i = 0; i < 7; i++) {
		for (int w = part1 ? 9 : 1; (part1 && w > 0) || (!part1 && w < 10); w += part1 ? -1 : 1) {
			if (part1 && w + d[i].n < 10 || !part1 && w + d[i].n > 0) {
				model += w * pow(10, d[i].i1);
				model += (w + d[i].n) * pow(10, d[i].i2);
				break;
			}
		}
	}

	return invert_long(model);
}

long
invert_long(long n)
{
	long inv = 0;

	while (n != 0) {
		inv = inv * 10 + n % 10;
		n /= 10;
	}

	return inv;
}
