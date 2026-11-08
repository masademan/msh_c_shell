#include <stdio.h>

#include "shell_command_edit.h"
#include "shell_command_move.h"

void render_typed_command_after_removing_char(const char *line, size_t line_length, size_t position,
                                              size_t amount_removed) {
    for (size_t i = position; i < line_length; i++) {
        putchar(line[i]);
    }

    for (size_t removed = 0; removed < amount_removed; removed++) {
        putchar(' ');
    }

    for (size_t i = position; i < line_length + amount_removed; i++) {
        move_cursor_left();
    }
}

void delete_characters_at_position_in_line(char *line, size_t *line_length, size_t position,
                                           size_t amount_to_remove) {
    if (position >= *line_length) {
        return;
    }
    if (amount_to_remove == 0) {
        return;
    }
    if (*line_length - position < amount_to_remove) {
        amount_to_remove = *line_length - position;
    }

    for (size_t i = position; i < *line_length - amount_to_remove; i++) {
        line[i] = line[i + amount_to_remove];
    }

    *line_length = *line_length - amount_to_remove;
}

void insert_char_into_line(char *line, size_t *line_length, size_t *position, char c) {
    if (*position != *line_length) {
        for (size_t i = *line_length + 1; i > *position; i--) {
            line[i] = line[i - 1];
        }
    }

    line[*position] = c;
}


void show_line_changes(char *line, size_t *line_length, size_t *position, char c) {
    if (*position == *line_length) {
        putchar(c);
    } else {
        render_typed_command_after_removing_char(line, *line_length, *position - 1, 0);
        move_cursor_right();
    }
}
