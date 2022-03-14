// String manipulation program for the Advent of Code 2021
// Written by Henry Peaurt on Feb 2022

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "strmanip.h"
#include "memmanage.h"

static char *
str_check(char *s)
{
	return s == NULL ? mem_alloc(1) : s;
}

char *
str_insert(char *s1, char *s2, size_t p)
{
	s1 = str_check(s1);
	s2 = str_check(s2);

	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	char *s_final = mem_alloc((len1 + len2 + 1) * sizeof(char));
	size_t i, j, k;
	i = j = k = 0;

	while (i < p)
		s_final[i++] = s1[j++];

	while (k < len2)
		s_final[i++] = s2[k++];

	while (j < len1)
		s_final[i++] = s1[j++];

	s_final[i] = '\0';
	return s_final;
}

char *
str_remove(char *s, size_t p1, size_t p2)
{
	s = str_check(s);

	if (p1 > p2) {
		size_t tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	size_t len = strlen(s);
	char *s_final = mem_realloc(s, (len + 1) * sizeof(char));
	size_t i = 0;
	size_t j = 0;

	while (i < p1)
		s_final[i++] = s[j++];

	j = p2 + 1;

	while (i < len)
		s_final[i++] = s[j++];

	s_final[i] = '\0';
	return s_final;
}

char *
str_append(char *s1, char *s2)
{
	s1 = str_check(s1);
	return str_insert(s1, s2, strlen(s1));
}

char *
str_push(char *s, char c)
{
	s = str_check(s);

	size_t len = strlen(s);

	s = mem_realloc(s, (len + 2) * sizeof(char));

	s[len] = c;
	s[len + 1] = '\0';

	return s;
}

char
str_pop(char *s)
{
	s = str_check(s);

	size_t len = strlen(s);
	size_t i = len > 0 ? len - 1 : 0;
	char c = s[i];

	s[i] = '\0';
	return c;
}

char *
str_input(void)
{
	char *s = NULL;
	char c = getchar();
	size_t i, cap;
	i = cap = 0;

	while (c != '\n' && c != EOF) {
		if (i >= cap)
			s = mem_realloc(s, (cap += 1024) * sizeof(char));

		s[i++] = c;
		c = getchar();
	}

	if (s != NULL && i == cap)
		s = mem_realloc(s, (cap += 512) * sizeof(char));

	if (s != NULL) s[i] = '\0';
	return s;
}

static bool
iswordchar(char c)
{
	return isalnum(c) || c == '-' || c == '\'';
}

char *
str_word(int n, char *s)
{
	char *w = NULL;
	int i = 0;

	// Skip words until n
	for (;n > 0; n--) {
		while (iswordchar(s[i]))
			i++;
		while (!iswordchar(s[i]))
			i++;
	}

	size_t len, cap;
	len = cap = 0;

	while (iswordchar(s[i])) {
		if (len == cap)
			w = mem_realloc(w, (cap += 20) * sizeof(char));

		w[len++] = s[i++];
	}

	if (w != NULL) w[len] = '\0';
	return w;
}

int
str_to_int_array(char *s, int **a)
{
	s = str_push(s, ' '); // The non-digit character makes the loop clean and stores the number
	size_t len = strlen(s);
	int cnt = 0;
	int cap = 0;
	int n = 0;
	bool storing_n = false;

	for (size_t i = 0; i < len; ++i) {
		if (isdigit(s[i])) {
			n = n * 10 + s[i] - '0';
			storing_n = true;
		} else if (storing_n) {
			if (cnt == cap)
				(*a) = mem_realloc((*a), (cap += 256) * sizeof(int));

			(*a)[cnt++] = n;
			n = 0;
			storing_n = false;
		}
	}

	return cnt;
}

char *
str_copy(char *s)
{
	size_t len = strlen(s);
	char *cp = mem_alloc((len + 1) * sizeof(char));

	memcpy(cp, s, len);

	return cp;
}

char *
str_reverse(char *s)
{
	size_t len = strlen(s);
	char *rv = mem_alloc((len + 1) * sizeof(char));

	for (size_t i = 0; i < len; i++)
		rv[i] = s[len - 1 - i];

	return rv;
}

char *
str_extract(char *s, size_t p1, size_t p2)
{
	size_t len = p2 - p1;
	char *ext = mem_alloc((len + 1) * sizeof(char));

	for (size_t i = 0; i < len; ++i)
		ext[i] = s[p1 + i];

	ext[len] = '\0';
	return ext;
}

int
chr_hex_to_int(char h)
{
	if (isdigit(h)) {
		return h - '0';
	} else {
		h = tolower(h);
		return h - 87;
	}
}

int
str_hex_to_int(char *s)
{
	int n = 0;

	for (int i = 0; s[i] != '\0'; i++)
		n += chr_hex_to_int(s[i]);

	return n;
}

int
str_bin_to_int(char *s)
{
	int n = 0;

	for (int i = 0; s[i] != '\0'; i++)
		n = s[i] == '1' ? ~(~n << 1) : n << 1;

	return n;
}

char *
str_int_to_str(int n)
{
	char *s = mem_alloc(16 * sizeof(char));
	int cap = 16;

	for (int i = 0; n > 0; ++i) {
		if (i == cap) {
			cap += 16;
			s = mem_realloc(s, cap * sizeof(char));
		}

		s[i] = n % 10 + '0';
		n /= 10;
	}

	return str_reverse(s);
}

void
skip_lines(int n)
{
	for (int nl = 0; nl < n; nl += getchar() == '\n')
		;
}
