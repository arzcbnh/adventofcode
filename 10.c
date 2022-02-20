// Advent of Code 2021 - Day 10
// Written by Henry Peaurt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function declarations
int getLineAlt(char **line);
char findCorruptedPair(char **line, char **missing);
int isOpening(char c);
int isClosing(char c);
int scoreCorruptedPair(char corrupted);
int scoreMissing(char **missing, long **missing_scores, int score_amount);
long findMiddleMissingScore(long **missing_scores, int score_amount);


int main(void)
{
	char *line = NULL;
	char *missing = NULL;
	int corrupted_score = 0;
	long *missing_scores = NULL, score_amount = 0;

	while (getLineAlt(&line)) {
		char corrupted = findCorruptedPair(&line, &missing);
		corrupted_score += scoreCorruptedPair(corrupted);
		if (!corrupted) {
			score_amount = scoreMissing(&missing,
					&missing_scores, score_amount);
		}
	}
	printf("Part 1: %d\n", corrupted_score);
	printf("Part 2: %ld\n",findMiddleMissingScore(&missing_scores,
						     score_amount));
	free(line);
	free(missing);
	free(missing_scores);
	return 0;
}


int getLineAlt(char **ptoline)
{
	int i;
	char c;

	for (i = 0; (c = getchar()) != '\n' && c != EOF; ++i) {
		*ptoline = realloc(*ptoline, sizeof(char) * (i + 2));
		(*ptoline)[i] = c;
	}
	(*ptoline)[i] = '\0';
	return i;
}


char findCorruptedPair(char **line, char **missing)
{
	int missing_amount = 0;
	char c;

	for (int position = 0; (c = (*line)[position]); ++position) {
		if (isOpening(c)) {
			*missing = realloc(*missing, sizeof(char) *
					 (missing_amount + 1));
			if (c == '(')
				(*missing)[missing_amount++] = c + 1;
			else
				(*missing)[missing_amount++] = c + 2;
		} else if (isClosing(c) && c != (*missing)[--missing_amount]) {
			break;
		}
	}
	(*missing)[missing_amount] = '\0';
	return c;
}
		

int isOpening(char c)
{
	return (c == '(' || c == '[' || c == '{' || c == '<');
}


int isClosing(char c)
{
	return (c == ')' || c == ']' || c == '}' || c == '>');
}


int scoreCorruptedPair(char corrupted)
{
	switch (corrupted) {
	case ')':
		return 3;
	case ']':
		return 57;
	case '}':
		return 1197;
	case '>':
		return 25137;
	}
	return 0;
}


int scoreMissing(char **missing, long **missing_scores, int score_amount)
{
	int missing_amount = strlen(*missing);
	long score = 0;

	while (missing_amount) {
		score *= 5;
		switch ((*missing)[--missing_amount]) {
		case '>':
			++score;
		case '}':
			++score;
		case ']':
			++score;
		case ')':
			++score;
		}
	}
	*missing_scores = realloc(*missing_scores, sizeof(long) *
				   (score_amount + 1));
	(*missing_scores)[score_amount++] = score;
	return score_amount;
}


long findMiddleMissingScore(long **missing_scores, int score_amount)
{
	int smaller, bigger;
	int i, j;

	for (i = 0; i < score_amount; ++i) {
		smaller = bigger = 0;
		for (j = 0; j < score_amount; ++j) {
			if ((*missing_scores)[j] < (*missing_scores)[i])
				++smaller;
			else if ((*missing_scores)[j] > (*missing_scores)[i])
				++bigger;
		}
		if (smaller == bigger)
			break;
	}
	return (*missing_scores)[i];
}
