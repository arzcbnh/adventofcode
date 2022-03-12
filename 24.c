// Advend of Code - Day 24
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strmanip.h"
#include "memmanage.h"

// Types
typedef struct { int a, b, c; } OpData;

// Function declarations
void input_data(OpData d[]);
long find_model(OpData *d, long z, int depth);

int
main(void)
{
	OpData d[15];
	input_data(d);

	printf("Part 1: %lu\n", find_model(d, 0, 1));

	/*
	for (int i = 0; i < 14; i++)
		printf("%i %i %i\n", d[i].a, d[i].b, d[i].c);
		*/

	return 0;
}

void
input_data(OpData d[])
{
	for (int i = 0; i < 14; i++) {
		// These for loops skip lines.
		for (int n = 0; n < 4; n += getchar() == '\n')
			;
		
		d[i].a = atoi(str_word(2, str_input()));
		d[i].b = atoi(str_word(2, str_input()));

		for (int n = 0; n < 9; n += getchar() == '\n')
			;

		d[i].c = atoi(str_word(2, str_input()));

		for (int n = 0; n < 2; n += getchar() == '\n')
			;
	}
}

long
find_model(OpData *d, long z, int depth)
{
	if (depth >= 15) return 0;

	for (int w = 9; w > 0; w--) {
		int nz = z;
		int x = z % 26 + d->b != w;
		nz /= d->a;
		nz *= 25 * x + 1;
		nz += (w + d->c) * x;
		if (ok) nz = 26 * nz + w + d->c;

		if (d->a == 26 && nz % 26 != 0) continue;
		long model = find_model(d + 1, nz, depth + 1);

		if (model >= 0)
			return model * 10 + w;
	}

	return -1;
}

/*
#include <unistd.h>
long
find_model(Order *start, long model)
{
	long input = model;

	while (proc_model(start, input))
	{
		printf("%li\n", model);
		if (--model % 10 == 0)
			--model;
		input = next_model(model);
	}

	return model;
}

bool
proc_model(Order *start, long input)
{
	Order *curr = start;
	char w, x, y, z;
	w = x = y = z = 0;

	while (curr != NULL) {
		char *var_a, var_b;

		switch (curr->a) {
		case W:	var_a = &w; break;
		case X:	var_a = &x; break;
		case Y:	var_a = &y; break;
		case Z:	var_a = &z; break;
		}

		switch (curr->b) {
		case W:	var_b = w; break;
		case X:	var_b = x; break;
		case Y:	var_b = y; break;
		case Z:	var_b = z; break;
		default: var_b = curr->b; break;
		}

		switch (curr->op) {
		case INP: *var_a = input % 10; input /= 10; break;
		case ADD: *var_a += var_b; break;
		case MUL: *var_a *= var_b; break;
		case DIV: *var_a /= var_b; break;
		case MOD: *var_a %= var_b; break;
		case EQL: *var_a = *var_a == var_b; break;
		}

		curr = curr->next;
	}

	return z;
}

long
next_model(long model)
{
	long new_m = 0;

	for (int i = 0; i < 14; i++) {
		int n = n % 10;
		new_m = new_m * 10 + n;
		model /= 10;
	}

	return new_m;
}
*/
