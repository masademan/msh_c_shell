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
    if (history_idx >= da_count_string(command_history)) {
        return;
    }

    history_idx++;
}

size_t get_command_idx(void) { return history_idx; }

size_t get_history_length(void) { return da_count_string(command_history); }

void set_command_idx_to_next(void) {
    if (history_idx == 0) {
        return;
    }

    history_idx--;
}

bool set_command_idx(size_t idx) {
    if (idx >= da_count_string(command_history)) {
        return false;
    }

    history_idx = idx;
    return true;
}

bool get_command(CharArray *line) {
    (void) line;

    return false;
}
