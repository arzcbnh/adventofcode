// Advent of Code 2021 - Day 09
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Constants
#define NEWLINE '\n'


// Function declarations
void getHeightmap(void);
void findLowPoints(void);
void findLowPointNearWall(int i);
int sumRiskLevel(void);
int findBasins(int i);
int checkIfMeasured(int i);
void compareBasinSize(int size);
int multiplyBasinSize(void);


// Global variables
int *heightmap, x, y;
int *lowpoints, lowpoints_amount;
int *basins, *measured_points, measured_amount;


int main(void)
{
	getHeightmap();
	findLowPoints();
	printf("Part 1: %d\n", sumRiskLevel());

	basins = malloc(sizeof(int) * 3);
	memset(basins, 0, sizeof(int) * 3);
	for (int i = 0; i < lowpoints_amount; ++i) {
		compareBasinSize(findBasins(lowpoints[i]));
		measured_amount = 0;
	}
	printf("Part 2: %d\n", multiplyBasinSize());
	return 0;
}


void getHeightmap(void)
{
	char c;
	int i = 0;
	
	while ((c = getchar()) != EOF) {
		if (c == NEWLINE) {
			x = i;
			i = 0;
			++y;
			continue;
		}
		heightmap = realloc(heightmap, sizeof(int) * (y * x + i + 1));
		heightmap[y * x + i++] = c - '0';
	}
}


void findLowPoints(void)
{
	for (int i = 0; i < x * y; ++i) {
		if (i % x && (i - x + 1) % x && i - x >= 0 && i + x < x * y) {
			if (heightmap[i - 1] > heightmap[i] &&
			    heightmap[i + 1] > heightmap[i] &&
			    heightmap[i - x] > heightmap[i] &&
			    heightmap[i + x] > heightmap[i]) {
				lowpoints = realloc(lowpoints, sizeof(int) *
						    (lowpoints_amount + 1));
				lowpoints[lowpoints_amount++] = i;
			}
		} else {
			findLowPointNearWall(i);
		}
	}
}


void findLowPointNearWall(int i)
{
	struct {
		unsigned int left : 1;
		unsigned int right : 1;
		unsigned int up : 1;
		unsigned int down : 1;
	} is_there;

	is_there.left = !(!(i % x));
	is_there.right = !(!((i - x + 1) % x));
	is_there.up = i - x >= 0;
	is_there.down = i + x < x * y;

	if (is_there.left && heightmap[i - 1] <= heightmap[i])
		return;
	if (is_there.right && heightmap[i + 1] <= heightmap[i])
		return;
	if (is_there.up && heightmap[i - x] <= heightmap[i])
		return;
	if (is_there.down && heightmap[i + x] <= heightmap[i])
		return;
	lowpoints = realloc(lowpoints, sizeof(int) * (lowpoints_amount + 1));
	lowpoints[lowpoints_amount++] = i;
}


int sumRiskLevel(void)
{
	int sum = 0;

	for (int i = 0; i < lowpoints_amount; ++i)
		sum += heightmap[lowpoints[i]] + 1;
	return sum;
}


int findBasins(int i)
{
	int size = 0;

	if (heightmap[i] == 9 || checkIfMeasured(i))
		return 0;

	if (i % x && heightmap[i - 1] > heightmap[i])
		size += findBasins(i - 1);			
	if ((i - x + 1) % x && heightmap[i + 1] > heightmap[i])
		size += findBasins(i + 1);
	if (i - x >= 0 && heightmap[i - x] > heightmap[i])
		size += findBasins(i - x);
	if (i + x < x * y && heightmap[i + x] > heightmap[i])
		size += findBasins(i + x);
	return size + 1;
}


int checkIfMeasured(int i)
{
	for (int j = 0; j < measured_amount; ++j) {
		if (measured_points[j] == i)
			return 1;
	}
	measured_points = realloc(measured_points, sizeof(int) * 
				  (measured_amount + 1));
	measured_points[measured_amount++] = i;
	return 0;
}


void compareBasinSize(int size)
{
	int smallest;
	smallest = basins[0] < basins[1] ? 0 : 1;
	smallest = basins[smallest] < basins[2] ? smallest : 2;

	for (int i = 0; i < 3; ++i) {
		if (size > basins[smallest]) {
			basins[smallest] = size;
			return;
		}
	}
}


int multiplyBasinSize(void)
{
	int result = 1;

	for (int i = 0; i < 3; result *= basins[i++])
		;
	return result;
}
