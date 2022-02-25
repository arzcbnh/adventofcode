// Header of string manipulation program for the Advent of Code 2021
// Written by Henry Peaurt on Feb 2022

#ifndef STR_MANIP
#define STR_MANIP

char* str_insert(char *s1, char* s2, unsigned long p);
char* str_remove(char *s, unsigned long p1, unsigned long p2);
char* str_append(char *s1, char *s2);
char* str_push(char *s1, char c);
char str_pop(char *s);
char* str_input(void);
int str_to_int_array(char *s, int **a);
char* str_word(int n, char *s);
char* str_literal(char *str);

#endif
