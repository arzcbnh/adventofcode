// String manipulation header for the Advent of Code 2021
// Written by Henry Peaurt on Feb 2022

#ifndef STRMANIP
#define STRMANIP

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char *str_check(char *s)
{
	if (!s) {
		s = malloc(1);
		*s = '\0';
	}
	return s;
}

char *str_insert(char *s1, char* s2, unsigned long p)
{
	s1 = str_check(s1);
	s2 = str_check(s2);

	unsigned long len1 = strlen(s1);
	unsigned long len2 = strlen(s2);
	unsigned long size = len1 + len2 + 1;
	char *s_final = malloc(sizeof(char) * size);

	p = len1 ? p % (len1 + 1) : 0;

	unsigned long i = 0;
	unsigned long j = 0;
	while (i < p)
		s_final[i++] = s1[j++];

	unsigned long k = 0;
	while (k < len2)
		s_final[i++] = s2[k++];

	while (j < len1)
		s_final[i++] = s1[j++];

	s_final[i] = '\0';
	free(s1);
	free(s2);
	return s_final;
}

char *str_remove(char *s, unsigned long p1, unsigned long p2)
{
	s = str_check(s);

	if (p1 > p2) {
		unsigned long tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	unsigned long len = strlen(s);

	p1 = len ? p1 % len : 0;
	p2 = len ? p2 % len : 0;

	unsigned long size = len - (p1 - p2);
	char *s_final = malloc(sizeof(char) * size);

	unsigned long i = 0;
	unsigned long j = 0;
	while (i < p1)
		s_final[i++] = s[j++];

	j = p2 + 1;
	while (i < size)
		s_final[i++] = s[j++];

	s_final[i] = '\0';
	free(s);
	return s_final;
}

char *str_append(char *s1, char *s2)
{
	s1 = str_check(s1);
	return str_insert(s1, s2, strlen(s1));
}

char *str_push(char *s1, char c)
{
	char* s2 = malloc(sizeof(char) * 2);

	s2[0] = c;
	s2[1] = '\0';

	return str_append(s1, s2);
}

char str_pop(char *s)
{
	s = str_check(s);

	unsigned long len = strlen(s);
	unsigned long p = len ? len - 1 : 0;
	char c = s[p];

	s[p] = '\0';
	return c;
}

char *str_input(void)
{
	unsigned long cap = 0;
	char *s = NULL;

	unsigned long i = 0;
	char c = '\0';
	while ((c = getchar()) != '\n' && c != EOF) {
		if (i == cap)
			s = realloc(s, sizeof(char) * (cap += 100));
		s[i++] = c;
	}

	if (i == cap)
		s = realloc(s, cap += 1);
	s[i] = '\0';

	if (c == EOF) {
		free(s);
		s = NULL;
	}

	return s;
}

unsigned int str_to_int_array(char *s, int **a)
{
	s = str_push(s, ' '); // The blank character makes the loop clean and stores the number
	unsigned long len = strlen(s);
	int count = 0;
	int cap = 0;
	int n = 0;
	_Bool storing_n = false;

	for (unsigned int i = 0; i < len; ++i) {
		if (isdigit(s[i])) {
			n = n * 10 + s[i] - '0';
			storing_n = true;
		} else if (storing_n) {
			if (count == cap) (*a) = realloc((*a), sizeof(int) * (cap += 100));
			(*a)[count++] = n;
			n = 0;
			storing_n = false;
		}
	}
	free(s);

	return count;
}

char *str_word(int n, char *s)
{
	char *w = NULL;
	int i = 0;

	// Skip words until n
	while (n--) {
		while (!isspace(s[i]) && s[i]) // Doesn't skip if s[i] is null
			++i;
		while (isspace(s[i]))
			++i;
	}

	int len = 0;
	int cap = 0;

	while (!isspace(s[i]) && s[i]) {
		if (len == cap)
			w = realloc(w, sizeof(char) * (cap += 20));

		w[len++] = s[i++];
	}

	if (w) w[len] = '\0';
	return w;
}

#endif
