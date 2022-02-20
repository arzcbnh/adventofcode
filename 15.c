// Advent of Code 2021 - Day 15
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Types
typedef char* path;
typedef struct node {
	int x;
	int y;
	int 
} node;


// Global variables
int *grid, x, y;
node **open, **closed;
int open_amount, closed_amount;


// Function declarations
static void getGrid(void);


int main(void)
{
	getGrid();

	path safest = findPath();

	return 0;
}


static void getGrid(void)
{
	char c;

	for (int i = 0; (c = getchar()) != EOF; ++i) {
		if (c == '\n') {
			x = i - 1;
			i = -1;
			++y;
			continue;
		}
		grid = realloc(grid, sizeof(int) * (i + x * y + 1));
		grid[i + x * y] = c - '0';
	}
}


static path findPath(void)
{
	node start, end;

	start.x = 0; start.y = 0;
	end.x = x - 1; end.y = y - 1;
	
	processNodes(&start);
	
	while (!(compareNodes(&current, &end))) {
	

	}

}


static void processNode(node *a)
{
	open = realloc(open, sizeof(node*) * (open_amount + 1));
	open[open_amount++] = a;

	closed_amount = 0;
	for (int i = 0; i < open_amount; ++i) {
		a = open[i];
		if (a->y > 0 && isOpen(a->x, a->y - 1)) {
			closed = realloc(closed, sizeof(node*) * (closed_amount
								  + 1));
			closed[closed_amount++] = 
				 (a->y < y - 1 && isOpen(a->x, a->y + 1)) +
				 (a->x > 0 && isopen(a->x - 1, a->y)) +
				 (a->x < x - 1 && isOpen(a->x + 1, a->y));
	}
	closed = realloc(closed, sizeof(node*) * (closed_amount + neighbours));

bool compareNodes(node *a, node *b)
{
	return (!(a && b) || a->x != b->x || a->y != b->y)
}
