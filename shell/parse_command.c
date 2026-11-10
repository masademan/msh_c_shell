#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "helpers/commands/check.h"
#include "macros/shell_globals.h"
#include "parse_command.h"

// TODO: Make a "clean_line" function to remove '\' from the line to make parsing easier
//         This will also allow for more complex line-breaking rules in the reading stage

/* Cleans the incoming line from shell_read_command
   It would:
    Replace "\\" with '\'
    Replace "\\\n" with ""
    And do more
   This is meant to make parsing easier, as well as allow for multi-line commands
   This function returns a new pointer that allocates memory
   The caller owns the pointer returned and has to use free() on it to free it */
static CharArray *shell_clean_line(const CharArray *line) {
    const int SPACE_REPLACEMENT = -2;

    size_t line_len = da_count_char(line);

    CharArray *cleaned_line = da_new_char();
    da_resize_char(cleaned_line, line_len, '\0');

    size_t i;
    char c;
    char last_c_written = '\0';
    bool spaces_start = true;

    // Clean up newlines, backslashes (\), spaces, parens, braces, etc.
    line_check_reset();

    size_t offset = 0;
    for (i = 0; i < line_len; i++) {
        if (!da_get_char(line, i, &c)) {
            da_free_char(cleaned_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
        }

        check_next_char(c);

        if (spaces_start && c == ' ') {
            offset++;
            continue;
        } else {
            spaces_start = false;
        }

        if (has_unfinished_backslash()) {
            offset += 2;
        } else if ((has_unfinished_pipe() || has_unfinished_logical_operator()) && c == '\n') {
            offset++;
        } else if ((has_open_parens() || has_open_braces()) && c == '\n') {
            if (last_c_written != '(' && last_c_written != '{' && last_c_written != '\0' &&
                last_c_written != ';') {
                if (!da_set_char(cleaned_line, i - offset, ';')) {
                    da_free_char(cleaned_line);

                    fprintf(stderr, "-%s: da_set error", SHELL_NAME);
                    raise(SIGINT);
                }

                last_c_written = ';';
            } else {
                offset++;
            }
        } else if (has_open_quotes() && c == ' ') {
            if (!da_set_char(cleaned_line, i - offset, SPACE_REPLACEMENT)) {
                da_free_char(cleaned_line);

                fprintf(stderr, "-%s: da_set error", SHELL_NAME);
                raise(SIGINT);
            }
        } else {
            if (last_c_written == ';' && (c == ')' || c == '}')) {
                offset++;
            }

            if (!da_set_char(cleaned_line, i - offset, c)) {
                da_free_char(cleaned_line);

                fprintf(stderr, "-%s: da_set error", SHELL_NAME);
                raise(SIGINT);
            }

            last_c_written = c;
        }
    }

    // Clean up double spaces
    if (!da_remove_duplicate_char(cleaned_line, ' ')) {
        da_free_char(cleaned_line);

        fprintf(stderr, "-%s: da_remove_duplicate error", SHELL_NAME);
        raise(SIGINT);
    }

    if (!da_replace_char(cleaned_line, SPACE_REPLACEMENT, ' ')) {
        da_free_char(cleaned_line);

        fprintf(stderr, "-%s: da_replace error", SHELL_NAME);
        raise(SIGINT);
    }

    // Add spaces between parts as necessary (after ;, around &&, around ||, around |)
    CharArray *spaced_line = da_new_char();
    size_t cleaned_len = da_count_char(cleaned_line);

    if (!da_get_char(cleaned_line, 0, &c)) {
        da_free_char(cleaned_line);
        da_free_char(spaced_line);

        fprintf(stderr, "-%s: da_get error", SHELL_NAME);
        raise(SIGINT);
    }
    if (!da_add_char(spaced_line, c)) {
        da_free_char(cleaned_line);
        da_free_char(spaced_line);

        fprintf(stderr, "-%s: da_add error", SHELL_NAME);
        raise(SIGINT);
    }

    line_check_reset();
    check_next_char(c);

    char prev_c, next_c;
    for (size_t i = 1; i < cleaned_len - 1; i++) {
        // Getting chars
        if (!da_get_char(cleaned_line, i - 1, &prev_c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
        }
        if (!da_get_char(cleaned_line, i, &c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
        }
        if (!da_get_char(cleaned_line, i + 1, &next_c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
        }

        check_next_char(c);

        // Before adding char
        if (!has_open_quotes()) {
            if ((c == '&' && prev_c != ' ' && prev_c != '&') ||
                (c == '|' && prev_c != ' ' && prev_c != '|')) {
                if (!da_add_char(spaced_line, ' ')) {
                    da_free_char(cleaned_line);
                    da_free_char(spaced_line);

                    fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                    raise(SIGINT);
                }
            }
        }

        // Adding char
        if (!da_add_char(spaced_line, c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
        }

        // After adding char
        if (!has_open_quotes()) {
            if ((c == '&' && next_c != ' ' && next_c != '&') ||
                (c == '|' && next_c != ' ' && next_c != '|') || (c == ';' && next_c != ' ')) {
                if (!da_add_char(spaced_line, ' ')) {
                    da_free_char(cleaned_line);
                    da_free_char(spaced_line);

                    fprintf(stderr, "-%s: da_add error", SHELL_NAME);
                    raise(SIGINT);
                }
            }
        }
    }

    if (cleaned_len == 2) {
        if (!da_get_char(cleaned_line, cleaned_len - 1, &next_c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
        }
    }

    da_free_char(cleaned_line);

    if (cleaned_len > 1) {
        if (!da_add_char(spaced_line, next_c)) {
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
        }
    }


    char ending_c;
    if (!da_get_char(spaced_line, da_count_char(spaced_line) - 1, &ending_c)) {
        da_free_char(spaced_line);

        fprintf(stderr, "-%s: da_add error", SHELL_NAME);
        raise(SIGINT);
    }

    if (ending_c == ' ') {
        if (!da_pop_char(spaced_line, NULL)) {
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_pop error", SHELL_NAME);
            raise(SIGINT);
        }
    }

    return spaced_line;
}

/* This splits the line that comes from shell_clean_line and returns a
    dynamic array of Strings
    This is what is read to execute commands and run programs
    This function returns a new pointer that allocates memory
    The caller owns the pointer returned, as well as each subpointer,
    and has to call arg_free() on it to free it */
static StringArray *shell_split_line(CharArray *line) {
    (void) line;

    return NULL;
}

StringArray *shell_parse_line(CharArray *line) {
    CharArray *cleaned_line = shell_clean_line(line);
    StringArray *split_args = shell_split_line(cleaned_line);

    String *s_line = da_join_to_string(line);
    printf("\n\"\"\"\n%s\n\"\"\"\n", ss_cstr(s_line));
    ss_free(s_line);

    String *s_cleaned_line = da_join_to_string(cleaned_line);
    printf("\n\"\"\"\n%s\n\"\"\"\n", ss_cstr(s_cleaned_line));
    ss_free(s_cleaned_line);

    da_free_char(cleaned_line);
    return split_args;
}
