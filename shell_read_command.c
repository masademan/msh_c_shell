#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell_command_check.h"
#include "shell_helpers.h"
#include "shell_read_command.h"

// TODO: Make it so that multi-line commands are possible
//         Maybe instead of doing many checks, make a function
//           that determines if a command is valid and ready to
//           be inputted and ran

// Make a header and c file with a global var
// This can check the string to check if the command is complete
// Code: string_check(char)
// Then use: (bool) quotes_closed

// split this function into 2 pieces
// one that reads line by line (for multiline stuff, or single)
// and one that glues all those pieces together

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

    line_check_reset();

    while (1) {
        c = getchar();
        check_next_char(c);

        if ((c == EOF || c == '\n') && is_complete_command()) {
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
