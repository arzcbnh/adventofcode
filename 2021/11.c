// Advent of Code 2021 - Day 11
// Written by Henry Peaurt

/* Just found out about pattern matching, but I decided not to learn it right now. Also, there's the new memory
 * management header, but I didn't need to allocate anything this time */

#include <stdio.h>
#include <stdbool.h>

// Constants
#define LEFT	x > 0
#define RIGHT	x < 9
#define ABOVE	y > 0
#define BELOW	y < 9

// Function declarations
void get_octos(int oct[10][10]);
void step_octos(int oct[10][10]);
void flash_octos(int oct[10][10]);
int zero_octos(int oct[10][10]);

int main(void)
{
	int octos[10][10];

	get_octos(octos);

	int step_cnt = 0;
	int flash_cnt = 0;
	_Bool part1 = false;
	_Bool part2 = false;

	while (!part1 || !part2) {
		step_octos(octos);
		flash_octos(octos);

		int tmp_cnt = zero_octos(octos);
		flash_cnt += tmp_cnt;
		++step_cnt;

		if (step_cnt == 100) {
			printf("Part One: %u\n", flash_cnt);
			part1 = true;
		}

		if (tmp_cnt == 100) {
			printf("Part Two: %u\n", step_cnt);
			part2 = true;
		}
	}

	return 0;
}

void get_octos(int oct[10][10])
{
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x)
			oct[x][y] = getchar() - '0';

		getchar(); // Skip newline
	}
}

void step_octos(int oct[10][10])
{
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x)
			++oct[x][y];
	}
}

void flash_octos(int oct[10][10])
{
	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			if (oct[x][y] >= 10) {
				oct[x][y]	  = -10; // You can check which octopi flashed if they're below 0

				oct[x - 1][y]	  += LEFT;
				oct[x + 1][y]	  += RIGHT;
				oct[x][y - 1]	  += ABOVE;
				oct[x][y + 1]	  += BELOW;
				oct[x - 1][y - 1] += LEFT && ABOVE;
				oct[x + 1][y - 1] += RIGHT && ABOVE;
				oct[x - 1][y + 1] += LEFT && BELOW;
				oct[x + 1][y + 1] += RIGHT && BELOW;

				x = -1; // Reset the checking
				y = 0;
			}
		}
	}
}

int zero_octos(int oct[10][10])
{
	int flash_cnt = 0;

	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			if (oct[x][y] < 0) {
				oct[x][y] = 0;
				++flash_cnt;
			}
		}
	}

	return flash_cnt;
}
