#include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#include "check.h"
#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "macros/shell_globals.h"
#include "split.h"

static bool found_token_edge = false;

/* Checks if c is a specific character
   If so, then intercept it and insert str_to_add to the arguments list instead of adding
   c to the temp_argument list */
static bool check_char_c(char c, char c_checked, const char *str_to_add, StringArray *args,
                         CharArray *temp_arg) {
    if (c == c_checked) {
        String *joined_string = da_join_to_string(temp_arg);
        String *trimmed_string = ss_strip(joined_string);

        if (ss_length(trimmed_string) > 0) {
            if (!da_add_string(args, trimmed_string)) {
                da_free_string(args);
                da_free_char(temp_arg);
                ss_free(joined_string);
                ss_free(trimmed_string);

                fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                raise(SIGINT);
                return true;
            }
        }

        ss_free(trimmed_string);
        ss_free(joined_string);
        da_clear_char(temp_arg);

        String *str_added = ss_new(str_to_add);
        if (!da_add_string(args, str_added)) {
            da_free_string(args);
            da_free_char(temp_arg);
            ss_free(str_added);

            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
            return true;
        }

        ss_free(str_added);

        found_token_edge = true;
    }

    return false;
}

StringArray *shell_split_line(CharArray *line) {
    StringArray *arguments = da_new_string();
    CharArray *temp_argument = da_new_char();
    char c, next_c;
    bool add_char;

    line_check_reset();

    for (size_t i = 0; i < da_count_char(line); i++) {
        if (!da_get_char(line, i, &c)) {
            da_free_string(arguments);
            da_free_char(temp_argument);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
            return NULL;
        }

        next_c = '\0';
        if (i + 1 < da_count_char(line)) {
            if (!da_get_char(line, i + 1, &next_c)) {
                da_free_string(arguments);
                da_free_char(temp_argument);

                fprintf(stderr, "-%s: da_get error", SHELL_NAME);
                raise(SIGINT);
                return NULL;
            }
        }

        check_next_char(c);

        found_token_edge = false;
        add_char = false;

        // Char checking
        if (!has_open_quotes()) {
            if (check_char_c(c, '(', "(", arguments, temp_argument)) {
                return NULL;
            } else if (check_char_c(c, ')', ")", arguments, temp_argument)) {
                return NULL;
            } else if (check_char_c(c, '{', "{", arguments, temp_argument)) {
                return NULL;
            } else if (check_char_c(c, '}', "}", arguments, temp_argument)) {
                return NULL;
            } else if (check_char_c(c, '&', "&&", arguments, temp_argument)) {
                return NULL;
            } else if (c == '|') {
                if (next_c == ' ') {  // single |
                    if (check_char_c(c, '|', "|", arguments, temp_argument)) {
                        return NULL;
                    }

                    i++;
                } else if (next_c == '|') {  // double ||
                    if (check_char_c(c, '|', "||", arguments, temp_argument)) {
                        return NULL;
                    }

                    check_next_char('|');

                    i += 2;
                }

                check_next_char(' ');
            } else if (check_char_c(c, ';', ";", arguments, temp_argument)) {
                return NULL;
            } else if (!found_token_edge) {
                add_char = true;
            }

            if (c == ';') {
                check_next_char(' ');
                i++;
            } else if (c == '&') {
                check_next_char('&');
                check_next_char(' ');
                i += 2;
            }
        } else {
            add_char = true;
        }

        // Char adding
        if (add_char) {
            if (!da_add_char(temp_argument, c)) {
                da_free_string(arguments);
                da_free_char(temp_argument);

                fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                raise(SIGINT);
                return NULL;
            }
        }
    }

    if (da_count_char(temp_argument) > 0) {
        String *joined_string = da_join_to_string(temp_argument);
        String *trimmed_string = ss_strip(joined_string);

        if (ss_length(trimmed_string) > 0) {
            if (!da_add_string(arguments, trimmed_string)) {
                da_free_string(arguments);
                da_free_char(temp_argument);
                ss_free(joined_string);
                ss_free(trimmed_string);

                fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                raise(SIGINT);
                return NULL;
            }
        }

        ss_free(trimmed_string);
        ss_free(joined_string);
    }

    da_free_char(temp_argument);
    return arguments;
}
