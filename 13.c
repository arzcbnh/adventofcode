// Advent of Code 2021 - Day 13
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


// Types
typedef struct point {
	int x;
	int y;
} point;
typedef struct fold {
	char axis;
	int n;
} fold;


// Function declarations
int createPoints(point ***points, int *x, int *y);
int getCoordinate(point *coordinate);
void copyDots(point **points, int point_amount, bool **dots, int x);
int createFolds(fold ***folds);
int getInstruction(fold *instruction);
void foldPaper(bool **dots, int x, int y, char axis, int n);
void resizePaper(bool **dots, int x, int y, char axis, int n);
int countDots(bool *dots, int dot_amount);


int main(void)
{
	point **points = NULL;
	int x = 0, y = 0;
	int point_amount = createPoints(&points, &x, &y);

	bool *dots = calloc(++x * ++y, sizeof(bool));
	copyDots(points, point_amount, &dots, x);

	fold **folds = NULL;
	int fold_amount = createFolds(&folds);
	for (int i = 0; i < fold_amount; ++i) {
		foldPaper(&dots, x, y, folds[i]->axis, folds[i]->n);
		resizePaper(&dots, x, y, folds[i]->axis, folds[i]->n);
		if (folds[i]->axis == 'x')
			x -= x - folds[i]->n;
		else
			y -= y - folds[i]->n;

		if (i == 0)
			printf("Part 1: %d\n", countDots(dots, x * y));
	}

	printf("Part 2:");
	for (int i = 0; i < x * y; ++i) {
		if (!(i % x))
			putchar('\n');
		if (dots[i])
			putchar('#');
		else
			putchar('.');
	}
	putchar('\n');

	for (int i = 0; i < point_amount; ++i)
		free(points[i]);
	for (int i = 0; i < fold_amount; ++i)
		free(folds[i]);
	free(points);
	free(folds);
	free(dots);
	return 0;
}


int createPoints(point ***points, int *x, int *y)
{
	point coordinate;
	int point_amount = 0;
	
	while (getCoordinate(&coordinate)) {
		*points = realloc(*points, sizeof(point*) * (point_amount + 1));
		(*points)[point_amount] = malloc(sizeof(point));
		memcpy((*points)[point_amount++], &coordinate, sizeof(point));
		
		*x = coordinate.x > *x ? coordinate.x : *x;
		*y = coordinate.y > *y ? coordinate.y : *y;
	}
	return point_amount;
}


int getCoordinate(point *coordinate)
{
	char c;

	coordinate->x = coordinate->y = 0;
	while ((c = getchar()) != ',') {
		if (c == '\n')
			return 0;
		coordinate->x = coordinate->x * 10 + c - '0';
	}
	while ((c = getchar()) != '\n')
		coordinate->y = coordinate->y * 10 + c - '0';
	return 1;
}


void copyDots(point **points, int point_amount, bool **dots, int x)
{
	for (int i = 0; i < point_amount; ++i) {
		int pt_x = points[i]->x;
		int pt_y = points[i]->y;
		(*dots)[pt_y * x + pt_x] = true;
	}
}


int createFolds(fold ***folds)
{
	fold instruction;
	int fold_amount = 0;

	while (getInstruction(&instruction)) {
		*folds = realloc(*folds, sizeof(fold*) * (fold_amount + 1));
		(*folds)[fold_amount] = malloc(sizeof(fold));
		memcpy((*folds)[fold_amount++], &instruction, sizeof(fold));
	}
	return fold_amount;
}


int getInstruction(fold *instruction)
{
	char c;

	while ((c = getchar()) != 'x' && c != 'y') {
		if (c == EOF)
			return 0;
	}
	instruction->axis = c == 'x' ? 'x' : 'y';
	getchar();
	instruction->n = 0;
	while (isdigit(c = getchar()))
		instruction->n = instruction->n * 10 + c - '0';
	return 1;
}


void foldPaper(bool **dots, int x, int y, char axis, int n)
{
	int dot_amount = x * y;

	for (int j = 0; j < dot_amount; ++j) {
		int line = j / x;
		int column = j % x;
		int n_2 = n + n;
		if (axis == 'x' && column < n) {
			bool *opposite = &(*dots)[(n_2 - column) + line * x];
			(*dots)[j] |= *opposite;
			*opposite = false;
		} else if (axis == 'y' && line < n) {
			bool *opposite = &(*dots)[(n_2 - line) * x + column];
			(*dots)[j] |= *opposite;
			*opposite = false;
		}
	}
}


void resizePaper(bool **dots, int x, int y, char axis, int n)
{
	int dot_amount = x * y;
	int a, b;

	if (axis == 'x') {
		int position = 0;
		for (int i = 0; i < dot_amount; ++i) {
			if (i % x < n)
				(*dots)[position++] = (*dots)[i];
		}
		a = x;
		b = y;
	} else {
		a = y;
		b = x;
	}
	*dots = realloc(*dots, sizeof(bool) * (dot_amount - (a - n) * b));
}


int countDots(bool *dots, int dot_amount)
{
	int true_dots = 0;

	for (int i = 0; i < dot_amount; ++i)
		true_dots += dots[i];
	return true_dots;
}
