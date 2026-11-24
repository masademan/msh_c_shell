#include <stdio.h>
#include <string.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "environ_var_expand.h"
#include "get_info/environ_var.h"
#include "macros/shell_globals.h"
#include "shell/helpers/helpers.h"

bool expand_the_environ_var_val(CharArray *temp_environ_var, CharArray *expanded_arg) {
    String *environ_var = da_join_to_string(temp_environ_var);
    char *environ_var_val = get_environ_var_val(ss_cstr(environ_var));
    ss_free(environ_var);

    if (!da_add_all_char(expanded_arg, environ_var_val, strlen(environ_var_val))) {
        da_free_char(expanded_arg);
        da_free_char(temp_environ_var);

        fprintf(stderr, "-%s: da_add_all error", SHELL_NAME);
        return false;
    }

    da_clear_char(temp_environ_var);

    return true;
}

CharArray *expand_environ_vars(const CharArray *cleaned_arg) {
    CharArray *expanded_arg = da_new_char();

    char c;
    if (!da_get_char(cleaned_arg, 0, &c)) {
        da_free_char(expanded_arg);

        fprintf(stderr, "-%s: da_get error", SHELL_NAME);
        return false;
    }
    if (da_count_char(cleaned_arg) == 1 && c == '~') {
        char *environ_var_val = get_environ_var_val("HOME");

        if (!da_add_all_char(expanded_arg, environ_var_val, strlen(environ_var_val))) {
            da_free_char(expanded_arg);

            fprintf(stderr, "-%s: da_add_all error", SHELL_NAME);
            return false;
        }

        return expanded_arg;
    }
    CharArray *temp_environ_var = da_new_char();

    size_t cleaned_len = da_count_char(cleaned_arg);
    da_reserve_char(expanded_arg, cleaned_len);

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

                if (!expand_the_environ_var_val(temp_environ_var, expanded_arg)) {
                    da_free_char(expanded_arg);
                    da_free_char(temp_environ_var);

                    return NULL;
                }

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

                if (da_count_char(temp_environ_var) == 1 && c == '0') {
                    if (!da_add_char(expanded_arg, '-')) {
                        da_free_char(expanded_arg);
                        da_free_char(temp_environ_var);

                        fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                        return NULL;
                    }
                    if (!da_add_all_char(expanded_arg, SHELL_NAME, strlen(SHELL_NAME))) {
                        da_free_char(expanded_arg);
                        da_free_char(temp_environ_var);

                        fprintf(stderr, "-%s: da_add_all error", SHELL_NAME);
                        return NULL;
                    }

                    da_clear_char(temp_environ_var);

                    found_environ_var = false;

                    continue;
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
        if (!expand_the_environ_var_val(temp_environ_var, expanded_arg)) {
            da_free_char(expanded_arg);
            da_free_char(temp_environ_var);

            return NULL;
        }
    }

    da_free_char(temp_environ_var);

    return expanded_arg;
}
