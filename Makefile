test: strtest.out
	@echo "abcde 12345" | ./strtest.out

strtest.out: strtest.o
	@gcc strtest.o -o strtest.out

strtest.o: strmanip.h strtest.c
	@gcc -g -std=c99 -pedantic -W -Wall -Werror -lm -c strtest.c -o strtest.o
