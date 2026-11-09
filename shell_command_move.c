#include <stdio.h>
#include <stdlib.h>

#include "dynamic_array_char.h"
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

size_t amount_to_move_cursor_to_next_left_word(const CharArray *line, size_t position) {
    size_t i;
    char c = 32;
    bool word_letter_seen = false;

    for (i = position; i > 0; i--) {
        if (i < da_count_char(line) && !da_get_char(line, i, &c)) {
            perror("da_get error");
            exit(EXIT_FAILURE);
        }

        if (word_letter_seen && i < da_count_char(line) && !is_word_letter(c)) {
            break;
        }

        if (is_word_letter(c) && i == position) {
            continue;
        }

        word_letter_seen = word_letter_seen || is_word_letter(c);
    }

    if (!is_word_letter(c)) {
        i++;
    }

    return position - i;
}

size_t amount_to_move_cursor_to_next_right_word(const CharArray *line, size_t position) {
    char c;
    size_t i;
    bool word_letter_seen = false;

    for (i = position; i < da_count_char(line); i++) {
        if (!da_get_char(line, i, &c)) {
            perror("da_get error");
            exit(EXIT_FAILURE);
        }

        if (word_letter_seen && !is_word_letter(c)) {
            break;
        }

        word_letter_seen = word_letter_seen || is_word_letter(c);
    }

    return i - position;
}

size_t move_cursor_to_next_left_word(const CharArray *line, size_t *position) {
    size_t shift_amount = amount_to_move_cursor_to_next_left_word(line, *position);

    move_cursor_left_by(shift_amount);
    *position = *position - shift_amount;

    return shift_amount;
}

size_t move_cursor_to_next_right_word(const CharArray *line, size_t *position) {
    size_t shift_amount = amount_to_move_cursor_to_next_right_word(line, *position);

    move_cursor_right_by(shift_amount);
    *position = *position + shift_amount;

    return shift_amount;
}
