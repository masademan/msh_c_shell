#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell_command_check.h"
#include "shell_helpers.h"
#include "shell_read_command.h"

char *shell_read_line(void) {
    size_t size = 8;
    size_t position = 0;
    char *line = malloc(size * sizeof(char));
    char *new_line;
    int c;

    // Setup error
    if (line == NULL) {
        perror("Initial COMMAND alloc failed");
        exit(EXIT_FAILURE);
    }

    // Sizing check
    if (position >= size) {
        while (position >= size) {
            size = size_increase_func(size);
            if (size < 1) {
                size = 1;
            }
        }

        new_line = realloc(line, size);
        if (new_line == NULL) {
            free(line);
            perror("Expanding COMMAND alloc failed");
            exit(EXIT_FAILURE);
        }

        line = new_line;
    }


    // Line reading
    while (1) {
        c = getchar();
        check_next_char(c);

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

char *shell_read_command(void) {
    size_t size = 8;
    char *command = malloc(size * sizeof(char));
    char *new_command;
    char *new_line;
    size_t command_size = 0;
    strcpy(command, "\0");

    line_check_reset();

    do {
        if (command_size != 0) {
            printf("> ");
        }

        new_line = shell_read_line();
        command_size += strlen(new_line) + 1;

        if (command_size > size) {
            new_command = realloc(command, command_size);
            if (new_command == NULL) {
                free(command);
                free(new_line);
                perror("Expanding COMMAND alloc failed");
                exit(EXIT_FAILURE);
            }

            command = new_command;
        }

        if (strlen(command) > 0) {
            strcat(command, "\n");
        }

        strcat(command, new_line);

        free(new_line);
    } while (!is_complete_command());

    return command;
}
