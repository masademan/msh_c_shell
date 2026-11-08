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

/* Returns true if the input character (int) is a letter in a word (like A-Z and a-z, or numbers)
   Else, returns false */
static bool is_word_letter(int c) {
    if ((65 <= c && c <= 90) || (97 <= c && c <= 122)) {  // English letters
        return true;
    }

    if (48 <= c && c <= 57) {  // Numbers
        return true;
    }

    return false;
}

size_t amount_to_move_cursor_to_next_left_word(const char *line, size_t line_length, size_t position) {
    size_t i;
    bool word_letter_seen = false;

    for (i = position; i > 0; i--) {
        if (word_letter_seen && i < line_length && !is_word_letter(line[i])) {
            break;
        }

        if (is_word_letter(line[i]) && i == position) {
            continue;
        }

        word_letter_seen = word_letter_seen || is_word_letter(line[i]);
    }

    if (!is_word_letter(line[i])) {
        i++;
    }

    return position - i;
}

size_t amount_to_move_cursor_to_next_right_word(const char *line, size_t line_length, size_t position) {
    size_t i;
    bool word_letter_seen = false;

    for (i = position; i < line_length; i++) {
        if (word_letter_seen && !is_word_letter(line[i])) {
            break;
        }

        word_letter_seen = word_letter_seen || is_word_letter(line[i]);
    }

    return i - position;
}

size_t move_cursor_to_next_left_word(const char *line, size_t line_length, size_t *position) {
    size_t shift_amount = amount_to_move_cursor_to_next_left_word(line, line_length, *position);

    move_cursor_left_by(shift_amount);
    *position = *position - shift_amount;

    return shift_amount;
}

size_t move_cursor_to_next_right_word(const char *line, size_t line_length, size_t *position) {
    size_t shift_amount = amount_to_move_cursor_to_next_right_word(line, line_length, *position);

    move_cursor_right_by(shift_amount);
    *position = *position + shift_amount;

    return shift_amount;
}
