// Advent of Code 2021 - Day 03
// Written by Henry Peaurt

/* The program is longer and more complex, so I thought it was better to start using context structs */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmanip.h"

// Types
typedef struct context {
	char **bins;
	int count;
	unsigned long bitlen;
} context;

// Function declarations
void store_bins(context *c);
char *calc_bit_frequency(context *c);
int bin_to_int(char *b);
char *find_unique_bin(context *c, char opt1, char opt2);

int main(void)
{
	context c;
	c.bins = NULL;
	c.count = 0;
	
	store_bins(&c);

	c.bitlen = strlen(*c.bins);

	char *bin = calc_bit_frequency(&c);
	int gamma = bin_to_int(bin);
	free(bin);

	bin = find_unique_bin(&c, '1', '0');
	int oxy_rate = bin_to_int(bin);

	bin = find_unique_bin(&c, '0', '1');
	int co2_rate = bin_to_int(bin);
	
	for (int i = 0; i < c.count; ++i)
		free(c.bins[i]);
	free(c.bins);

	printf("Part 1: %u\n", gamma * (~(~0U << c.bitlen) & ~gamma));
	printf("Part 2: %u\n", oxy_rate * co2_rate);
	return 0;
}

void store_bins(context *c)
{
	char *bin;
	int cap = 0;

	while ((bin = str_input())) {
		if (c->count == cap)
			c->bins = realloc(c->bins, sizeof(char*) * (cap += 50));

		c->bins[c->count++] = bin;
	}
}

char *calc_bit_frequency(context *c)
{
	char *bin = calloc(c->bitlen + 1, sizeof(char));

	for (int i = 0; i < c->count; ++i) {
		for (unsigned int j = 0; j < c->bitlen; ++j)
			bin[j] += (c->bins[i])[j] - '0' ? 1 : -1;
	}

	for (unsigned int i = 0; i < c->bitlen; ++i)
		bin[i] = bin[i] >= 0 ? '1' : '0';

	return bin;
}

int bin_to_int(char *b)
{
	unsigned long bitlen = strlen(b);
	int n = 0;

	for (unsigned int i = 0; i < bitlen; ++i)
		n = b[i] - '0' ? ~(~n << 1) : n << 1;

	return n;
}

char *find_unique_bin(context *c, char opt1, char opt2)
{
	char **copy = malloc(sizeof(char*) * c->count);
	for (int i = 0; i < c->count; ++i)
		copy[i] = c->bins[i];

	int left = c->count;
	for (int cur_bit = 0; left > 1; ++cur_bit) {
		int freq = 0;
		for (int i = 0; i < left; ++i)
			freq += (copy[i])[cur_bit] - '0' ? 1 : -1;

		char common = freq >= 0 ? opt1 : opt2;
		int tmp_left = 0;
		for (int i = 0; i < left; ++i) {
			if ((copy[i])[cur_bit] == common)
				copy[tmp_left++] = copy[i];
		}
		left = tmp_left;
	}

	char *b = *copy;
	free(copy);
	return b;
}
