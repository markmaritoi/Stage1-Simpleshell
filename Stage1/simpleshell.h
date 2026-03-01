#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

#include <stdio.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

// prompt + env
void print_prompt(void);
void set_shell_env(char *argv0);

// parsing
int parse_args(char *buf, char *args[], int max_args);

// internal commands
// returns: 0 not internal, 1 handled, 2 quit
int handle_internal(int argc, char **args);

#endif

/*
Name: Andrew Mark Maritoi
Student Number: 27617
Academic Integrity: I acknowledge the DCU Academic Integrity Policy.
*/