// Advent of Code 2021 - Day 02
// Written by Henry Peaurt

/* Really simple, everything can be processed very quickly */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmanip.h"
#include "memmanage.h"

int main(void)
{
	long depth1 = 0;
	long depth2 = 0;
	long position = 0;
	long aim = 0;
	char *order;

	while ((order = str_input())) {
		if (strstr(order, "forward")) {
			int n = order[8] - '0';
			position += n;
			depth2 += aim * n;
		} else if (strstr(order, "up")) {
			int n = order[3] - '0';
			depth1 -= n;
			aim -= n;
		} else if (strstr(order, "down")) {
			int n = order[5] - '0';
			depth1 += n;
			aim += n;
		}
	}

	printf("Part 1: %lu\n", position * depth1);
	printf("Part 2: %lu\n", position * depth2);

	mem_clean();

	return 0;
}
