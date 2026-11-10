#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "edit.h"
#include "macros/shell_globals.h"
#include "move.h"

void render_typed_command_after_removing_char(const CharArray *line, size_t position,
                                              size_t amount_removed) {
    char c;
    for (size_t i = position; i < da_count_char(line); i++) {
        if (!da_get_char(line, i, &c)) {
            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
        }

        putchar(c);
    }

    for (size_t removed = 0; removed < amount_removed; removed++) {
        putchar(' ');
    }

    for (size_t i = position; i < da_count_char(line) + amount_removed; i++) {
        move_cursor_left();
    }
}

void delete_characters_at_position_in_line(CharArray *line, size_t position, size_t amount_to_remove) {
    if (position >= da_count_char(line)) {
        return;
    }
    if (amount_to_remove == 0) {
        return;
    }
    if (da_count_char(line) - position < amount_to_remove) {
        amount_to_remove = da_count_char(line) - position;
    }

    da_remove_n_char(line, position, amount_to_remove);
}

void show_line_changes(const CharArray *line, size_t *position, char c) {
    if (*position == da_count_char(line)) {
        putchar(c);
    } else {
        render_typed_command_after_removing_char(line, *position - 1, 0);
        move_cursor_right();
    }
}
