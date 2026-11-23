#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "clean.h"
#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/safe_string/safe_string.h"
#include "environ_var_expand.h"
#include "macros/shell_globals.h"
#include "shell/helpers/commands/check.h"

/* Check if c is value_check, and if so, replace it with value_replace
   Then increment offset
   This is for replacing backslash escape characters with their respective character */
static void handle_escape_chars(char *c, char value_check, char value_replace, size_t *offset) {
    if (*c == value_check) {
        *c = value_replace;
        (*offset)++;
    }
}

void clean_argument(String **arg_string) {
    CharArray *arg_char_arr = ss_split_to_char_array(*arg_string);
    size_t offset = 0;
    bool had_open_quotes = false;
    bool had_open_backslashes = false;
    bool found_escape_char = false;
    char c;

    line_check_reset();

    for (size_t i = 0; i < da_count_char(arg_char_arr); i++) {
        if (!da_get_char(arg_char_arr, i, &c)) {
            da_free_char(arg_char_arr);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            return;
        }

        check_next_char(c);

        // Backslash escape char check
        if (found_escape_char) {
            if (has_open_quotes()) {
                handle_escape_chars(&c, '\"', '\"', &offset);
                handle_escape_chars(&c, '\'', '\'', &offset);
                // handle_escape_chars(&c, 'n', '\n', &offset);
                // handle_escape_chars(&c, 't', '\t', &offset);
                // handle_escape_chars(&c, '\\', '\\', &offset);
                // handle_escape_chars(&c, '$', '$', &offset);
            } else if (c != '$') {
                offset++;
            }

            found_escape_char = false;
        }

        if (!had_open_backslashes && has_open_backslash()) {
            found_escape_char = true;
        }
        had_open_backslashes = has_open_backslash();

        if (!da_set_char(arg_char_arr, i - offset, c)) {
            da_free_char(arg_char_arr);

            fprintf(stderr, "-%s: da_set error", SHELL_NAME);
            return;
        }

        // Opening/closing quotes check
        if ((!had_open_quotes && has_open_quotes()) || (had_open_quotes && !has_open_quotes())) {
            offset++;
        }

        had_open_quotes = has_open_quotes();
    }

    da_remove_n_char(arg_char_arr, da_count_char(arg_char_arr) - offset, offset);

    // ss_free(*arg_string);
    // *arg_string = da_join_to_string(arg_char_arr);
    // da_free_char(arg_char_arr);

    CharArray *environ_var_expanded_arg_arr = expand_environ_vars(arg_char_arr);
    da_free_char(arg_char_arr);

    ss_free(*arg_string);
    *arg_string = da_join_to_string(environ_var_expanded_arg_arr);

    da_free_char(environ_var_expanded_arg_arr);
}
