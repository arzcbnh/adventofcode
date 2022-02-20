// Advent of Code 2021 - Day 11
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Constants
#define FLASHED		-10
#define LEFT		!(!(i % 10))
#define RIGHT		!(!((i + 1) % 10))
#define ABOVE		i - 10 >= 0
#define BELOW		i + 10 < 100


// Function declarations
void getOctopuses(int *octopuses);
void stepOctopuses(int *octopuses);
void flashOctopuses(int *octopuses);
int zeroOctopuses(int *octopuses);


int main(void)
{
	int *octopuses = malloc(sizeof(int) * 100);
	int *octopuses_backup = malloc(sizeof(int) * 100);
	int flash_amount = 0;
	int step;

	getOctopuses(octopuses);
	memcpy(octopuses_backup, octopuses, sizeof(int) * 100);
	for (step = 0; step < 100; ++step) {
		stepOctopuses(octopuses);
		flashOctopuses(octopuses);
		flash_amount += zeroOctopuses(octopuses);
	}
	printf("Part One: %d\n", flash_amount);

	memcpy(octopuses, octopuses_backup, sizeof(int) * 100);
	for (step = 0; flash_amount != 100; ++step) {
		stepOctopuses(octopuses);
		flashOctopuses(octopuses);
		flash_amount = zeroOctopuses(octopuses);
	}
	printf("Part Two: %d\n", step);

	return 0;
}


void getOctopuses(int *octopuses)
{
	char c;

	for (int i = 0; (c = getchar()) != EOF; ++i) {
		if (c == '\n') {
			--i;
			continue;
		}
		octopuses[i] = c - '0';
	}
}


void stepOctopuses(int *octopuses)
{
	for (int i = 0; i < 100; ++i)
		++octopuses[i];
}


void flashOctopuses(int *octopuses)
{
	for (int i = 0; i < 100; ++i) {
		if (octopuses[i] >= 10) {
			octopuses[i] = FLASHED;
			octopuses[i - 11] += LEFT && ABOVE;
			octopuses[i - 10] += ABOVE;
			octopuses[i - 9] += RIGHT && ABOVE;
			octopuses[i - 1] += LEFT;
			octopuses[i + 1] += RIGHT;
			octopuses[i + 9] += LEFT && BELOW;
			octopuses[i + 10] += BELOW;
			octopuses[i + 11] += RIGHT && BELOW;
			i = -1;
		}
	}
}


int zeroOctopuses(int *octopuses)
{
	int flash_amount = 0;

	for (int i = 0; i < 100; ++i) {
		if (octopuses[i] < 0) {
			octopuses[i] = 0;
			++flash_amount;
		}
	}
	return flash_amount;
}
