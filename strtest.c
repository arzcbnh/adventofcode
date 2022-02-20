// Tests for the string management header

#include <assert.h>
#include <stdio.h>

#include "strmanip.h"

int main(void)
{
	char *s = NULL;
/*
	puts("str_insert in nothing...");
	s = str_insert("", "abcde", 3); // Any number should work
	assert(!(strcmp(s, "abcde")));

	puts("str_insert in the beginning...");
	s = str_insert("abcde", "12345", 0);
	assert(!(strcmp(s, "12345abcde")));

	puts("str_insert in the middle...");
	s = str_insert("abcde", "12345", 2);
	assert(!(strcmp(s, "ab12345cde")));

	puts("str_append / str_insert in the end...");
	s = str_insert("abcde", "12345", 5);
	assert(!(strcmp(s, "abcde12345")));
	s = str_append("abcde", "12345");
	assert(!(strcmp(s, "abcde12345")));

	puts("str_remove from nothing...");
	s = str_remove("", 4, 21); // Any number should work
	assert(!(strcmp(s, "")));

	puts("str_remove from the beginning...");
	s = str_remove("abcde", 0, 2);
	assert(!(strcmp(s, "de")));

	puts("str_remove from the middle...");
	s = str_remove("abcde", 1, 3);
	assert(!(strcmp(s, "ae")));

	puts("str_remove from the end...");
	s = str_remove("abcde", 2, 4);
	assert(!(strcmp(s, "ab")));

	puts("str_insert with extrapolated index...");
	s = str_insert("abcde", "12345", 14);
	assert(!(strcmp(s, "ab12345cde")));
	
	puts("str_push / str_append...");
	s = str_push("abcde", '1');
	assert(!(strcmp(s, "abcde1")));

	puts("str_pop...");
	s = str_append("", "abcde");
	char c = str_pop(s);
	assert(!(strcmp(s, "abcd")));
	assert(c == 'e');

	puts("str_input...");
	s = str_input();
	assert(!(strcmp(s, "abcde 12345")));
*/
	puts("str_word...");
	s = str_word(0, "hi hello");
	printf("%s\n", s);
	assert(!strcmp(s, "hi"));
	s = str_word(2, "please help me");
	printf("%s\n", s);
	assert(!strcmp(s, "me"));
	s = str_word(10, "lmao");
	assert(s == NULL);
	
	return 0;
}
