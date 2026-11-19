#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "clean.h"
#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/safe_string/safe_string.h"
#include "get_info/environ_var.h"
#include "macros/shell_globals.h"
#include "shell/helpers/commands/check.h"

/* Returns true if c is a number (0-9)
   Else, returns false */
static bool char_is_num(char c) { return '0' <= c && c <= '9'; }

/* Returns true if c is a letter (a-z, A-Z) or an underscore (_) */
static bool char_is_alpha_or_underscore(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_');
}


/* Takes in a clean argument (from clean_argument) and expands any environment variables necessary
   Caller owns the returns pointer and has to call da_free_char() on it to free it */
static CharArray *expand_environ_vars(const CharArray *cleaned_arg) {
    CharArray *expanded_arg = da_new_char();
    CharArray *temp_environ_var = da_new_char();

    size_t cleaned_len = da_count_char(cleaned_arg);
    da_reserve_char(expanded_arg, cleaned_len);

    char c;
    bool found_escape_char = false;
    bool found_environ_var = false;
    for (size_t i = 0; i < cleaned_len; i++) {
        if (!da_get_char(cleaned_arg, i, &c)) {
            da_free_char(expanded_arg);
            da_free_char(temp_environ_var);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            return NULL;
        }

        if (found_environ_var) {
            if (!char_is_num(c) && !char_is_alpha_or_underscore(c)) {
                found_environ_var = false;

                if (da_count_char(temp_environ_var) == 0) {
                    if (!da_add_char(expanded_arg, '$')) {
                        da_free_char(expanded_arg);
                        da_free_char(temp_environ_var);

                        fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                        return NULL;
                    }
                    if (!da_add_char(expanded_arg, c)) {
                        da_free_char(expanded_arg);
                        da_free_char(temp_environ_var);

                        fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                        return NULL;
                    }

                    continue;
                }

                String *environ_var = da_join_to_string(temp_environ_var);
                char *environ_var_val = get_environ_var_val(ss_cstr(environ_var));
                ss_free(environ_var);

                if (!da_add_all_char(expanded_arg, environ_var_val, strlen(environ_var_val))) {
                    da_free_char(expanded_arg);
                    da_free_char(temp_environ_var);

                    fprintf(stderr, "-%s: da_add_all error", SHELL_NAME);
                    return NULL;
                }

                da_clear_char(temp_environ_var);

                if (c == '$') {
                    found_environ_var = true;
                }
            } else {
                if (!da_add_char(temp_environ_var, c)) {
                    da_free_char(expanded_arg);
                    da_free_char(temp_environ_var);

                    fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                    return NULL;
                }
            }
        } else if (found_escape_char) {
            if (c == '\\' || c == '$') {
                da_pop_char(expanded_arg, NULL);
            }

            found_escape_char = false;
        } else if (c == '\\') {
            found_escape_char = true;
        } else if (c == '$') {
            found_environ_var = true;
        }

        if (!found_environ_var) {
            if (!da_add_char(expanded_arg, c)) {
                da_free_char(expanded_arg);
                da_free_char(temp_environ_var);

                fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                return NULL;
            }
        }
    }

    if (da_count_char(temp_environ_var) > 0) {
        String *environ_var = da_join_to_string(temp_environ_var);
        char *environ_var_val = get_environ_var_val(ss_cstr(environ_var));
        ss_free(environ_var);

        if (!da_add_all_char(expanded_arg, environ_var_val, strlen(environ_var_val))) {
            da_free_char(expanded_arg);
            da_free_char(temp_environ_var);

            fprintf(stderr, "-%s: da_add_all error", SHELL_NAME);
            return NULL;
        }
    }

    return expanded_arg;
}

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

    CharArray *environ_var_expanded_arg_arr = expand_environ_vars(arg_char_arr);
    da_free_char(arg_char_arr);

    ss_free(*arg_string);

    *arg_string = da_join_to_string(environ_var_expanded_arg_arr);

    da_free_char(environ_var_expanded_arg_arr);
}
