#include <stdio.h>

#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "history.h"

static size_t history_idx;
static StringArray *command_history;

void setup_command_history(void) {
    command_history = da_new_string();
    history_idx = 0;
}

void clean_up_command_history(void) { da_free_string(command_history); }

void add_command(const CharArray *command) {
    String *command_str = da_join_to_string(command);
    da_add_string(command_history, command_str);
    ss_free(command_str);
}

void pop_command(void) { da_pop_string(command_history, NULL); }

void set_command_idx_to_prev(void) {
    if (history_idx + 1 >= da_count_string(command_history)) {
        return;
    }

    history_idx++;
}

void set_command_idx_to_next(void) {
    if (history_idx == 0) {
        return;
    }

    history_idx--;
}

size_t get_command_idx(void) { return history_idx; }

size_t get_history_length(void) { return da_count_string(command_history); }

bool set_command_idx(size_t idx) {
    if (idx >= da_count_string(command_history)) {
        return false;
    }

    history_idx = idx;
    return true;
}

bool get_command(CharArray *line) {
    if (da_count_string(command_history) == 0 || line == NULL) {
        return false;
    }

    String *curr_command;
    if (!da_get_string(command_history, da_count_string(command_history) - history_idx - 1,
                       &curr_command)) {
        return false;
    }

    ss_split_copy_to_char_array(curr_command, line);

    return true;
}

bool has_duplicate_at_2_newest_commands(void) {
    String *prior_command, *curr_command;

    if (!da_get_string(command_history, da_count_string(command_history) - 1, &curr_command)) {
        return false;
    }
    if (!da_get_string(command_history, da_count_string(command_history) - 2, &prior_command)) {
        return false;
    }

    return ss_equals(prior_command, curr_command);
}

void print_history(void) {
    String *command;

    printf("{\n");
    for (size_t i = 0; i < da_count_string(command_history); i++) {
        da_get_string(command_history, i, &command);

        printf("  %s,\n", ss_cstr(command));
    }
    printf("}\n");
}
