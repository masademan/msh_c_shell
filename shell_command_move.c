#include <stdio.h>

#include "shell_command_move.h"

void move_cursor_left(void) { printf("\033[D"); }

void move_cursor_right(void) { printf("\033[C"); }

void move_cursor_left_by(size_t shift_amount) {
    for (size_t i = 0; i < shift_amount; i++) {
        move_cursor_left();
    }
}

void move_cursor_right_by(size_t shift_amount) {
    for (size_t i = 0; i < shift_amount; i++) {
        move_cursor_right();
    }
}

void move_cursor_to(size_t *curr_position, size_t end) {
    for (size_t i = *curr_position; i < end; i++) {  // Going right
        move_cursor_right();
    }
    for (size_t i = end; i < *curr_position; i++) {  // Going left
        move_cursor_left();
    }

    *curr_position = end;
}

/* Returns true if the input character (int) is an ASCII letter (like A-Z and a-z)
   Else, returns false */
static bool is_ASCII_char(int c) { return (65 <= c && c <= 90) || (97 <= c && c <= 122); }

size_t amount_to_move_cursor_to_next_left_word(const char *line, size_t line_length, size_t position) {
    (void) line;
    (void) line_length;
    (void) position;

    return 0;
}

size_t amount_to_move_cursor_to_next_right_word(const char *line, size_t line_length, size_t position) {
    (void) line;
    (void) line_length;
    (void) position;

    return 0;
}

size_t move_cursor_to_next_left_word(const char *line, size_t line_length, size_t *position) {
    (void) line;
    (void) line_length;
    (void) position;

    return 0;
}

size_t move_cursor_to_next_right_word(const char *line, size_t line_length, size_t *position) {
    (void) line;
    (void) line_length;
    (void) position;

    return 0;
}
