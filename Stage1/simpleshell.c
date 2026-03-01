#include "simpleshell.h"

#include <stdio.h>

int main(int argc, char **argv) {
    set_shell_env(argv[0]);

    FILE *in = stdin;
    int batch_mode = 0;

    if (argc == 2) {
        in = fopen(argv[1], "r");
        if (!in) {
            perror("batchfile fopen");
            return 1;
        }
        batch_mode = 1;
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [batchfile]\n", argv[0]);
        return 1;
    }

    char buf[MAX_BUFFER];
    char *args[MAX_ARGS];

    while (1) {
        if (!batch_mode) print_prompt();

        if (!fgets(buf, MAX_BUFFER, in)) break; // EOF

        int argc_parsed = parse_args(buf, args, MAX_ARGS);
        if (argc_parsed == 0) continue;

        int internal = handle_internal(argc_parsed, args);
        if (internal == 2) break;     // quit
        if (internal == 1) continue;  // handled

        fprintf(stderr, "Unsupported command in Stage 1: %s\n", args[0]);
    }

    if (batch_mode && in != stdin) fclose(in);
    return 0;
}

/*
Name: Andrew Mark Maritoi
Student Number: 27617
Academic Integrity: I acknowledge the DCU Academic Integrity Policy.
*/