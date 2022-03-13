# Advent of Code 2021 in C

To use a solution, download the repository, save the input for the solution you want to use in a file named `input` in the repository folder, and run the shell script with the name of the program. Here's different ways you can run the script:

```
$ ./run 13.c
$ ./run -t 05.c
$ ./run -gv 22.c
```

Always write flags before the name of the program. You can use the following flags:

- `-g`: forces compilation with GCC.
- `-s`: simple compilation, without pedantic and warnings.
- `-t`: inputs a file called `test` instead of `input`.
- `-v`: runs with Valgrind.
