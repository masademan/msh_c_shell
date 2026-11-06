#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell_helpers.h"
#include "shell_read_line.h"

char *shell_read_line(void) {
    size_t size = 8;
    size_t position = 0;
    char *line = malloc(size * sizeof(char));
    char *new_line;
    int c;

    if (line == NULL) {
        perror("Initial COMMAND alloc failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            line[position] = '\0';
            break;
        } else {
            line[position] = c;
        }
        ++position;

        if (position >= size) {
            size = size_increase_func(size);
            if (size < 1) {
                size = 1;
            }

            new_line = realloc(line, size);
            if (new_line == NULL) {
                free(line);
                perror("Expanding COMMAND alloc failed");
                exit(EXIT_FAILURE);
            }

            line = new_line;
        }
    }

    return line;
}
