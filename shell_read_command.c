#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "dynamic_array_char.h"
#include "shell_command_check.h"
#include "shell_command_edit.h"
#include "shell_command_move.h"
#include "shell_read_command.h"

// TODO: make shell history work
//       maybe tab complete

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
static bool keep_reading_command;
static void sigint_handler(int signum) {
    (void) signum;

    keep_reading_command = false;
    // Use this if you want the shell to look like BASH, with the ^C after doing Ctrl + C
    // printf("^C");
    printf("\n");
}

/* Handles arrow keys and the delete key */
static CharArray *shell_handle_ANSI(CharArray *line, size_t *position) {
    int c = getchar();
    CharArray *char_a = da_new_char();

    // printf("\n(%d)", c);

    if (c == 'A') {  // Up arrow
        // TODO:
        // get prev history

        da_free_char(char_a);
        return NULL;
    } else if (c == 'B') {  // Down arrow
        // TODO:
        // load next history

        da_free_char(char_a);
        return NULL;
    } else if (c == 'C') {  // Right arrow
        if (*position < da_count_char(line)) {
            move_cursor_right();
            *position = *position + 1;
        }

        da_free_char(char_a);
        return NULL;
    } else if (c == 'D') {  // Left arrow
        if (*position > 0) {
            move_cursor_left();
            *position = *position - 1;
        }

        da_free_char(char_a);
        return NULL;
    } else if (c == '3') {  // Maybe Del key
        c = getchar();
        da_add_char(char_a, c);

        if (c == '~') {  // Del key
            if (*position < da_count_char(line)) {
                delete_characters_at_position_in_line(line, *position, 1);

                render_typed_command_after_removing_char(line, *position, 1);
            }

            da_free_char(char_a);
            return NULL;
        } else if (c == ';') {
            c = getchar();
            da_add_char(char_a, c);

            if (c == '5') {
                c = getchar();
                da_add_char(char_a, c);

                if (c == '~') {  // Ctrl + Del key
                    if (*position < da_count_char(line)) {
                        size_t shift_amount = amount_to_move_cursor_to_next_right_word(line, *position);
                        delete_characters_at_position_in_line(line, *position, shift_amount);
                        render_typed_command_after_removing_char(line, *position, shift_amount);
                    }

                    da_free_char(char_a);
                    return NULL;
                }
            }
        }
    } else if (c == '1') {  // Maybe home key
        c = getchar();
        da_add_char(char_a, c);

        if (c == '~') {  // Home key
            move_cursor_to(position, 0);

            da_free_char(char_a);
            return NULL;
        } else if (c == ';') {
            c = getchar();
            da_add_char(char_a, c);

            if (c == '5') {
                c = getchar();
                da_add_char(char_a, c);

                if (c == 'D') {  // Ctrl + Left arrow
                    move_cursor_to_next_left_word(line, position);

                    da_free_char(char_a);
                    return NULL;
                } else if (c == 'C') {  // Ctrl + Right arrow
                    move_cursor_to_next_right_word(line, position);

                    da_free_char(char_a);
                    return NULL;
                }
            }
        }
    } else if (c == '4') {  // Maybe end key
        c = getchar();
        da_add_char(char_a, c);

        if (c == '~') {  // End key
            move_cursor_to(position, da_count_char(line));

            da_free_char(char_a);
            return NULL;
        }
    }

    return char_a;
}

/* Reads a single character into memory, and also handles backspace, delete, and arrow keys */
static int shell_read_char(CharArray *line, size_t *position) {
    int c = getchar();

    // printf("\n(%d)", c);

    if (c == '\033') {  // ANSI
        getchar();      // Skips the [ part of the ANSI code

        CharArray *char_a = shell_handle_ANSI(line, position);
        if (char_a != NULL) {
            da_add_char(line, '\033');
            da_add_char(line, '[');
            da_extend_char(line, char_a);
            da_free_char(char_a);
        }

        return CONTINUE_CODE;
    } else if (c == 127) {  // Backspace key
        if (*position > 0) {
            delete_characters_at_position_in_line(line, *position - 1, 1);

            move_cursor_left();
            *position = *position - 1;

            render_typed_command_after_removing_char(line, *position, 1);
        }

        return CONTINUE_CODE;
    } else if (c == 23 || c == 8) {  // Ctrl + W and Ctrl + Backspace
        if (*position > 0) {
            size_t shift_amount = move_cursor_to_next_left_word(line, position);
            delete_characters_at_position_in_line(line, *position, shift_amount);
            render_typed_command_after_removing_char(line, *position, shift_amount);
        }

        return CONTINUE_CODE;
    } else if (c == 4) {  // Ctrl + D (Like ctrl + del)
        size_t shift_amount = amount_to_move_cursor_to_next_right_word(line, *position);
        delete_characters_at_position_in_line(line, *position, shift_amount);
        render_typed_command_after_removing_char(line, *position, shift_amount);
        return CONTINUE_CODE;
    } else if (c == 1) {  // Ctrl + A (Like the home key)
        move_cursor_to(position, 0);
        return CONTINUE_CODE;
    } else if (c == 5) {  // Ctrl + E (Like the end key)
        move_cursor_to(position, da_count_char(line));
        return CONTINUE_CODE;
    } else if (c == 27) {  // Alt + B (Like ctrl + left arrow)
        move_cursor_to_next_left_word(line, position);
        return CONTINUE_CODE;
    } else if (c == 102) {  // Alt + F (Like ctrl + right arrow)
        move_cursor_to_next_right_word(line, position);
        return CONTINUE_CODE;
    } else if (c == 9) {  // Tab
        // do stuff
        return CONTINUE_CODE;
    }

    return c;
}

CharArray *shell_read_line(void) {
    size_t position = 0;
    CharArray *line = da_new_char();
    int c;

    da_reserve_char(line, 8);

    // Setup error
    if (line == NULL) {
        perror("Initial COMMAND alloc failed");
        exit(EXIT_FAILURE);
    }

    enable_raw_mode();

    // Line reading
    while (keep_reading_command) {
        c = shell_read_char(line, &position);

        if (c == CONTINUE_CODE) {
            continue;
        }

        if (c == EOF || c == '\n') {
            putchar(c);
            break;
        } else {
            da_insert_char(line, position, c);
            ++position;
        }

        show_line_changes(line, &position, c);
    }

    disable_raw_mode();

    char ith_char;
    for (size_t i = 0; i < da_count_char(line); i++) {
        if (!da_get_char(line, i, &ith_char)) {
            perror("line da_get");
            exit(EXIT_FAILURE);
        }
        check_next_char(ith_char);
    }

    return line;
}

CharArray *shell_read_command(void) {
    CharArray *command = da_new_char();
    CharArray *new_line;

    da_reserve_char(command, 8);

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
        if (da_count_char(command) != 0) {
            printf("> ");
        }

        new_line = shell_read_line();

        if (!keep_reading_command) {
            da_free_char(new_line);
            break;
        }

        if (da_count_char(command) > 0) {
            da_add_char(command, '\n');
        }

        da_extend_char(command, new_line);

        da_free_char(new_line);
    } while (!is_complete_command());

    if (!keep_reading_command) {
        da_free_char(command);
        return NULL;
    }

    return command;
}
