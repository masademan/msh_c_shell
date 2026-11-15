#include <stdbool.h>
#include <stdio.h>

#include "clean.h"
#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "macros/shell_globals.h"
#include "shell/helpers/commands/check.h"

void clean_argument(String **arg_string) {
    CharArray *arg_char_arr = ss_split_to_char_array(*arg_string);
    size_t offset = 0;
    bool had_open_quotes = false;
    char c;

    line_check_reset();

    for (size_t i = 0; i < da_count_char(arg_char_arr); i++) {
        if (!da_get_char(arg_char_arr, i, &c)) {
            da_free_char(arg_char_arr);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            return;
        }

        check_next_char(c);

        if (!da_set_char(arg_char_arr, i - offset, c)) {
            da_free_char(arg_char_arr);

            fprintf(stderr, "-%s: da_set error", SHELL_NAME);
            return;
        }

        if ((!had_open_quotes && has_open_quotes()) || (had_open_quotes && !has_open_quotes())) {
            offset++;
        }

        had_open_quotes = has_open_quotes();
    }

    da_remove_n_char(arg_char_arr, da_count_char(arg_char_arr) - offset, offset);

    ss_free(*arg_string);

    *arg_string = da_join_to_string(arg_char_arr);

    return;
}
