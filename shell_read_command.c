#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "shell_command_check.h"
#include "shell_helpers.h"
#include "shell_read_command.h"

struct termios orig_termios;

// AI generated to allow for backspace, delete, and arrow keys (including history func)
static void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    // Disable canonical mode and terminal echo
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// AI generated to allow for backspace, delete, and arrow keys (including history func)
static void disable_raw_mode(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

/* Used to show the changes after a backspace, delete, or mid-line character insert */
static void render_typed_command_after_removing_char(char *line, size_t line_length, size_t position) {
    for (size_t i = position; i < line_length; i++) {
        putchar(line[i]);
    }

    putchar(' ');

    for (size_t i = position; i < line_length + 1; i++) {
        printf("\033[D");
    }
}

char *shell_read_line(void) {
    size_t size = 8;
    size_t position = 0;
    size_t line_length = 0;
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


    enable_raw_mode();

    // Line reading
    while (1) {
        c = getchar();
        check_next_char(c);

        if (c == '\033') {  // ANSI
            getchar();      // Skips the [ part of the ANSI code
            c = getchar();

            if (c == 'A') {  // Up arrow
                // TODO:
                // get prev history
            } else if (c == 'B') {  // Down arrow
                // TODO:
                // load next history
            } else if (c == 'C') {  // Right arrow
                if (position < line_length) {
                    printf("\033[C");
                    ++position;
                }
            } else if (c == 'D') {  // Left arrow
                if (position > 0) {
                    printf("\033[D");
                    --position;
                }
            } else if (c == '3') {  // Maybe Del key
                c = getchar();

                if (c == '~') {  // Del key
                    if (position < line_length) {
                        for (size_t i = position; i < line_length - 1; i++) {
                            line[i] = line[i + 1];
                        }

                        --line_length;

                        render_typed_command_after_removing_char(line, line_length, position);
                    }
                }
            }
            continue;
        } else if (c == 127) {  // Backspace key
            if (position > 0) {
                for (size_t i = position; i < line_length; i++) {
                    line[i - 1] = line[i];
                }

                printf("\033[D");
                --line_length;
                --position;

                render_typed_command_after_removing_char(line, line_length, position);
            }
            continue;
        }

        if (c == EOF || c == '\n') {
            line[line_length] = '\0';
            break;
        } else {
            if (position != line_length) {
                for (size_t i = line_length + 1; i > position; i--) {
                    line[i] = line[i - 1];
                }
            }

            line[position] = c;

            ++line_length;
            ++position;
        }

        if (position == line_length) {
            putchar(c);
        } else {
            render_typed_command_after_removing_char(line, line_length, position - 1);
            printf("\033[C");
        }

        if (line_length >= size) {
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

    disable_raw_mode();

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
