#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "shell_command_check.h"
#include "shell_command_edit.h"
#include "shell_command_move.h"
#include "shell_helpers.h"
#include "shell_read_command.h"

// TODO: refactor to use safe strings and dynamic arrays from class
//         a bit more work for better code, plus fun link back to those projects

#define CONTINUE_CODE -2

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

// Ctrl + C handler
static size_t ctrl_c_shift_right_amount;
static bool keep_reading_command;
static void sigint_handler(int signum) {
    (void) signum;

    keep_reading_command = false;
    // Use this if you want the shell to look like BASH, with the ^C after doing Ctrl + C
    // printf("^C");
    printf("\n");
}

/* Handles arrow keys and the delete key */
static void shell_handle_ANSI(char *line, size_t *line_length, size_t *position) {
    int c = getchar();

    // printf("\n(%d)", c);

    if (c == 'A') {  // Up arrow
        // TODO:
        // get prev history
    } else if (c == 'B') {  // Down arrow
        // TODO:
        // load next history
    } else if (c == 'C') {  // Right arrow
        if (*position < *line_length) {
            move_cursor_right();
            *position = *position + 1;
        }
    } else if (c == 'D') {  // Left arrow
        if (*position > 0) {
            move_cursor_left();
            *position = *position - 1;
        }
    } else if (c == '3') {  // Maybe Del key
        c = getchar();

        if (c == '~') {  // Del key
            if (*position < *line_length) {
                delete_characters_at_position_in_line(line, line_length, *position, 1);

                render_typed_command_after_removing_char(line, *line_length, *position, 1);
            }
        } else if (c == ';') {
            c = getchar();
            if (c == '5') {
                c = getchar();
                if (c == '~') {  // Ctrl + Del key
                    size_t shift_amount =
                        amount_to_move_cursor_to_next_right_word(line, *line_length, *position);

                    delete_characters_at_position_in_line(line, line_length, *position, shift_amount);
                }
            }
        }
    } else if (c == '1') {  // Maybe home key
        c = getchar();

        if (c == '~') {  // Home key
            move_cursor_to(position, 0);
        } else if (c == ';') {
            c = getchar();
            if (c == '5') {
                c = getchar();
                if (c == 'D') {  // Ctrl + Left arrow
                    move_cursor_to_next_left_word(line, *line_length, position);
                } else if (c == 'C') {  // Ctrl + Right arrow
                    move_cursor_to_next_right_word(line, *line_length, position);
                }
            }
        }
    } else if (c == '4') {  // Maybe end key
        c = getchar();

        if (c == '~') {  // End key
            move_cursor_to(position, *line_length);
        }
    }
}

/* Reads a single character into memory, and also handles backspace, delete, and arrow keys */
static int shell_read_char(char *line, size_t *line_length, size_t *position) {
    int c = getchar();

    // printf("\n(%d)", c);

    if (c == '\033') {  // ANSI
        getchar();      // Skips the [ part of the ANSI code
        shell_handle_ANSI(line, line_length, position);
        return CONTINUE_CODE;
    } else if (c == 127) {  // Backspace key
        if (*position > 0) {
            delete_characters_at_position_in_line(line, line_length, *position - 1, 1);

            move_cursor_left();
            *position = *position - 1;

            render_typed_command_after_removing_char(line, *line_length, *position, 1);
        }

        return CONTINUE_CODE;
    } else if (c == 23 || c == 8) {  // Ctrl + W and Ctrl + Backspace
        if (*position > 0) {
            size_t shift_amount = move_cursor_to_next_left_word(line, *line_length, position);
            delete_characters_at_position_in_line(line, line_length, *position, shift_amount);
        }

        return CONTINUE_CODE;
    } else if (c == 4) {  // Ctrl + D (Like ctrl + del)
        size_t shift_amount = amount_to_move_cursor_to_next_right_word(line, *line_length, *position);

        delete_characters_at_position_in_line(line, line_length, *position, shift_amount);
        return CONTINUE_CODE;
    } else if (c == 1) {  // Ctrl + A (Like the home key)
        move_cursor_to(position, 0);
        return CONTINUE_CODE;
    } else if (c == 5) {  // Ctrl + E (Like the end key)
        move_cursor_to(position, *line_length);
        return CONTINUE_CODE;
    } else if (c == 27) {  // Alt + B (Like ctrl + left arrow)
        move_cursor_to_next_left_word(line, *line_length, position);
        return CONTINUE_CODE;
    } else if (c == 102) {  // Alt + F (Like ctrl + right arrow)
        move_cursor_to_next_right_word(line, *line_length, position);
        return CONTINUE_CODE;
    }

    return c;
}

/* Increases the allocated memory for line and returns a new pointer */
static char *expand_line_malloced_memory(char *line, size_t *size) {
    *size = size_increase_func(*size);
    if (*size < 1) {
        *size = 1;
    }

    char *new_line = realloc(line, *size);
    if (new_line == NULL) {
        free(line);
        perror("Expanding COMMAND alloc failed");
        exit(EXIT_FAILURE);
    }

    return new_line;
}

char *shell_read_line(void) {
    size_t size = 8;
    size_t position = 0;
    size_t line_length = 0;
    char *line = malloc(size * sizeof(char));
    int c;

    // Setup error
    if (line == NULL) {
        perror("Initial COMMAND alloc failed");
        exit(EXIT_FAILURE);
    }

    enable_raw_mode();

    // Line reading
    while (keep_reading_command) {
        c = shell_read_char(line, &line_length, &position);

        ctrl_c_shift_right_amount = line_length - position;

        if (c == CONTINUE_CODE) {
            continue;
        }

        if (c == EOF || c == '\n') {
            putchar(c);
            line[line_length] = '\0';
            break;
        } else {
            insert_char_into_line(line, &line_length, &position, c);

            ++line_length;
            ++position;
        }

        show_line_changes(line, &line_length, &position, c);

        if (line_length >= size) {
            line = expand_line_malloced_memory(line, &size);
        }
    }

    disable_raw_mode();

    for (size_t i = 0; i < line_length; i++) {
        check_next_char(line[i]);
    }

    return line;
}

static char *expand_command_malloced_memory(char *command, char *new_line, size_t *size) {
    *size = size_increase_func(*size);
    if (*size < 1) {
        *size = 1;
    }

    char *new_command = realloc(command, *size);
    if (new_command == NULL) {
        free(command);
        free(new_line);
        perror("Expanding COMMAND alloc failed");
        exit(EXIT_FAILURE);
    }

    return new_command;
}

char *shell_read_command(void) {
    size_t size = 8;
    char *command = malloc(size * sizeof(char));
    char *new_line;
    size_t command_size = 0;
    strcpy(command, "\0");

    keep_reading_command = true;

    // AI generated code:
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    // signal(SIGINT, sigint_handler);

    line_check_reset();

    do {
        if (command_size != 0) {
            printf("> ");
        }

        new_line = shell_read_line();

        if (!keep_reading_command) {
            free(new_line);
            break;
        }

        command_size += strlen(new_line) + 1;

        if (command_size > size) {
            command = expand_command_malloced_memory(command, new_line, &size);
        }

        if (strlen(command) > 0) {
            strcat(command, "\n");
        }

        strcat(command, new_line);

        free(new_line);
    } while (!is_complete_command());

    if (!keep_reading_command) {
        free(command);
        return NULL;
    }

    return command;
}
