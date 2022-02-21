// Advent of Code 2021 - Day 10
// Written by Henry Peaurt

/* This time, I decided to process each line. I think I'll start processing everything at once if it's needed, but line
 * by line if it's possible to do so. I wonder if that's the reason the code turned out so ugly. I didn't use arrays
 * since the line length varies. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "strmanip.h"

// Types
typedef struct {
	long *list;
	int count;
	int cap;
} missing;

enum state {
	CORRUPTED,
	MISSING
};

typedef struct {
	char *str;
	enum state stt;
} analysis;

// Function declarations
analysis proc_line(char *s);
_Bool is_opening(char c);
int ret_score(char c, enum state s);
long calc_miss_score(char *s);
void push_miss_score(missing *m, long n);
int compare_scores(const void *a, const void *b);

int main(void)
{
	missing m = {
		.list = NULL,
		.count = 0,
		.cap = 0
	};
	char *s = NULL;
	long score = 0;

	while ((s = str_input())) {
		analysis a = proc_line(s);

		if (a.stt == CORRUPTED) {
			score += ret_score(*a.str, CORRUPTED);
			free(a.str);
		} else {
			push_miss_score(&m, calc_miss_score(a.str));
			free(a.str);
		}

		free(s);
	}

	qsort(m.list, m.count, sizeof(long long), compare_scores);

	printf("Part 1: %lu\n", score);
	printf("Part 2: %lu\n", m.list[m.count / 2]);

	free(m.list);

	return 0;
}

analysis proc_line(char *s)
{
	char *missing = NULL;
	char corrupt = '\0';
	unsigned long len = strlen(s);

	for (unsigned int i = 0; i < len; ++i) {
		char c = s[i];

		if (is_opening(c)) {
			c += c == '(' ? 1 : 2; // Turns into the closing character
			missing = str_push(missing, c);
		} else if (c != str_pop(missing)) {
			corrupt = c;
			break;
		}
	}

	char *copy = NULL;

	if (corrupt) {
		copy = malloc(sizeof(char) * 1);
		*copy = corrupt;
		free(missing);
	} else
		missing = str_push(missing, '\0');

	analysis a = {
		.str = corrupt ? copy : missing,
		.stt = corrupt ? CORRUPTED : MISSING
	};

	return a;
}
		
_Bool is_opening(char c)
{
	return (c == '(' || c == '[' || c == '{' || c == '<');
}

int ret_score(char c, enum state s)
{
	switch (c) {
	case ')':
		return s == MISSING ? 1 : 3;
	case ']':
		return s == MISSING ? 2 : 57;
	case '}':
		return s == MISSING ? 3 : 1197;
	case '>':
		return s == MISSING ? 4 : 25137;
	}

	return 0;
}

long calc_miss_score(char *s)
{
	long score = 0;

	for (unsigned long i = strlen(s); i; --i) {
		score *= 5;
		score += ret_score(s[i - 1], MISSING);
	}

	return score;
}

void push_miss_score(missing *m, long n)
{
	if (m->count == m->cap)
		m->list = realloc(m->list, sizeof(long) * (m->cap += 256));

	m->list[m->count++] = n;
}

int compare_scores(const void *a, const void *b)
{
	long arg1 = *(const long*) a;
	long arg2 = *(const long*) b;

	return (arg1 > arg2) - (arg1 < arg2);
}
