#include "simpleshell.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
// libraries 


     
extern char **environ; // The 'environ' global variable gives access to all environment variables for the current process.


/*

PRINT PROMPT:

Displays the current working directory as the shell prompt.
Uses getcwd() to retrieve the current directory path.
If getcwd fails, a fallback prompt is printed.
*/

void print_prompt(void) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s> ", cwd);
    } else {
        printf("?> ");
    }
    fflush(stdout);
}


/*

SET SHELL ENVIRONMENT VARIABLE:

Sets environment variable:
    shell=/full/path/to/simpleshell

Steps:
1. If argv0 already contains a '/', try to resolve it.
2. Otherwise search through PATH directories.
3. Store the absolute path using setenv().
*/

void set_shell_env(char *argv0) {
    char resolved[PATH_MAX];

    if (strchr(argv0, '/') != NULL) {
        if (realpath(argv0, resolved) != NULL) {
            setenv("shell", resolved, 1);
            return;
        }
    }

    char *path_env = getenv("PATH");
    if (!path_env) path_env = "";

    char *copy = strdup(path_env);
    if (!copy) {
        perror("strdup(PATH)");
        setenv("shell", argv0, 1);
        return;
    }

    char *saveptr = NULL;
    for (char *dir = strtok_r(copy, ":", &saveptr);
         dir != NULL;
         dir = strtok_r(NULL, ":", &saveptr)) {

        char candidate[PATH_MAX];
        snprintf(candidate, sizeof(candidate), "%s/%s", dir, argv0);

        if (access(candidate, X_OK) == 0) {
            if (realpath(candidate, resolved) != NULL) {
                setenv("shell", resolved, 1);
                free(copy);
                return;
            }
        }
    }

    free(copy);
    setenv("shell", argv0, 1);
}


/*

PARSE COMMAND LINE INPUT:

Splits user input into tokens using whitespace
(space, tab, newline) as separators.

Example:
Input: echo hello world
Output:
    args[0] = "echo"
    args[1] = "hello"
    args[2] = "world"

Returns number of arguments parsed.
*/

int parse_args(char *buf, char *args[], int max_args) {
    int argc_parsed = 0;
    char *tok = strtok(buf, SEPARATORS);
    while (tok && argc_parsed < max_args - 1) {
        args[argc_parsed++] = tok;
        tok = strtok(NULL, SEPARATORS);
    }
    args[argc_parsed] = NULL;
    return argc_parsed;
}

// ---- internal commands helpers ----

/*
CD COMMAND:

- If no argument: print current directory
- If argument present: change directory
- After successful change, update PWD environment variable
*/

static void cmd_cd(int argc, char **args) {
    if (argc == 1) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) printf("%s\n", cwd);
        else perror("cd/getcwd");
        return;
    }
    if (chdir(args[1]) != 0) {
        perror("cd");
        return;
    }
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (setenv("PWD", cwd, 1) != 0) perror("cd/setenv(PWD)");
    }
}


/*
CLR COMMAND:

Clears screen using system call to external 'clear' command.
*/

static void cmd_clr(void) {
    if (system("clear") == -1) perror("clr/system");
}

/*
DIR COMMAND:

Lists directory contents using:
    ls -al <directory>

If no directory specified, defaults to current directory (".").
*/

static void cmd_dir(int argc, char **args) {
    const char *dir = (argc >= 2) ? args[1] : ".";
    char cmd[PATH_MAX + 32];
    snprintf(cmd, sizeof(cmd), "ls -al \"%s\"", dir);
    if (system(cmd) == -1) perror("dir/system");
}

/*
ENVIRON COMMAND:

Prints all environment variables.
Uses the global 'environ' pointer.
*/

static void cmd_environ(void) {
    for (char **e = environ; *e != NULL; e++) {
        printf("%s\n", *e);
    }
}

/*
ECHO COMMAND:

Prints all arguments after 'echo'.
Multiple spaces are reduced due to tokenisation.
*/

static void cmd_echo(int argc, char **args) {
    for (int i = 1; i < argc; i++) {
        printf("%s", args[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
}

/*
HELP COMMAND:

Displays the user manual
*/

static void cmd_help(void) {
    if (system("more manual/readme.txt") == -1)
        perror("help");
}

/*
PAUSE COMMAND:

Suspends shell execution until user presses Enter.
*/

static void cmd_pause(void) {
    printf("Press Enter to continue...");
    fflush(stdout);
    int c;
    do { c = getchar(); } while (c != '\n' && c != EOF);
}

// returns: 0 not internal, 1 handled, 2 quit
int handle_internal(int argc, char **args) {
    if (strcmp(args[0], "cd") == 0)      { cmd_cd(argc, args); return 1; }
    if (strcmp(args[0], "clr") == 0)     { cmd_clr(); return 1; }
    if (strcmp(args[0], "dir") == 0)     { cmd_dir(argc, args); return 1; }
    if (strcmp(args[0], "environ") == 0) { cmd_environ(); return 1; }
    if (strcmp(args[0], "echo") == 0)    { cmd_echo(argc, args); return 1; }
    if (strcmp(args[0], "help") == 0)    { cmd_help(); return 1; }
    if (strcmp(args[0], "pause") == 0)   { cmd_pause(); return 1; }
    if (strcmp(args[0], "quit") == 0)    { return 2; }
    return 0;
}

/*
Name: Andrew Mark Maritoi
Student Number: 27617
Academic Integrity: I acknowledge the DCU Academic Integrity Policy.
*/