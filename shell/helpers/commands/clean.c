#include <signal.h>
#include <stddef.h>
#include <stdio.h>

#include "check.h"
#include "clean.h"
#include "macros/shell_globals.h"

// To implement an AST for more complex commands (conditionals + loops)
//  this function needs to be revised

/* Cleans most of the line in shell_clean_line
   Cleans the newlines, backslashes, spaces, parens, braces, etc... */
static void shell_clean_most(CharArray *cleaned_line, const CharArray *line,
                             const int SPACE_REPLACEMENT) {
    size_t line_len = da_count_char(line);

    size_t i;
    char c, next_c;
    char last_c_written = '\0';
    bool spaces_start = true;

    line_check_reset();

    size_t offset = 0;
    for (i = 0; i < line_len; i++) {
        if (!da_get_char(line, i, &c)) {
            da_free_char(cleaned_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
            return;
        }
        next_c = '\0';
        if (i + 1 < line_len) {
            if (!da_get_char(line, i + 1, &next_c)) {
                da_free_char(cleaned_line);

                fprintf(stderr, "-%s: da_get error", SHELL_NAME);
                raise(SIGINT);
                return;
            }
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
                    return;
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
                return;
            }
        } /* else if (!has_open_quotes() && (c == '\"' || c == '\'') &&
                    (next_c == '\"' || next_c == '\'')) {
             check_next_char(next_c);
             offset += 2;
             i++;
         }*/
        else {
            if (last_c_written == ';' && (c == ')' || c == '}')) {
                offset++;
            }

            if (!da_set_char(cleaned_line, i - offset, c)) {
                da_free_char(cleaned_line);

                fprintf(stderr, "-%s: da_set error", SHELL_NAME);
                raise(SIGINT);
                return;
            }

            last_c_written = c;
        }
    }

    // Clean up double spaces
    if (!da_remove_duplicate_char(cleaned_line, ' ')) {
        da_free_char(cleaned_line);

        fprintf(stderr, "-%s: da_remove_duplicate error", SHELL_NAME);
        raise(SIGINT);
        return;
    }

    if (!da_replace_char(cleaned_line, SPACE_REPLACEMENT, ' ')) {
        da_free_char(cleaned_line);

        fprintf(stderr, "-%s: da_replace error", SHELL_NAME);
        raise(SIGINT);
        return;
    }
}

static void shell_clean_syntax(CharArray *spaced_line, CharArray *cleaned_line) {
    size_t cleaned_len = da_count_char(cleaned_line);
    char c;

    if (!da_get_char(cleaned_line, 0, &c)) {
        da_free_char(cleaned_line);
        da_free_char(spaced_line);

        fprintf(stderr, "-%s: da_get error", SHELL_NAME);
        raise(SIGINT);
        return;
    }
    if (!da_add_char(spaced_line, c)) {
        da_free_char(cleaned_line);
        da_free_char(spaced_line);

        fprintf(stderr, "-%s: da_add error", SHELL_NAME);
        raise(SIGINT);
        return;
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
            return;
        }
        if (!da_get_char(cleaned_line, i, &c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
            return;
        }
        if (!da_get_char(cleaned_line, i + 1, &next_c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
            return;
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
                    return;
                }
            }
        }

        // Adding char
        if (!da_add_char(spaced_line, c)) {
            da_free_char(cleaned_line);
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
            return;
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
                    return;
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
            return;
        }
    }

    da_free_char(cleaned_line);

    if (cleaned_len > 1) {
        if (!da_add_char(spaced_line, next_c)) {
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_add error", SHELL_NAME);
            raise(SIGINT);
            return;
        }
    }


    char ending_c;
    if (!da_get_char(spaced_line, da_count_char(spaced_line) - 1, &ending_c)) {
        da_free_char(spaced_line);

        fprintf(stderr, "-%s: da_add error", SHELL_NAME);
        raise(SIGINT);
        return;
    }

    if (ending_c == ' ') {
        if (!da_pop_char(spaced_line, NULL)) {
            da_free_char(spaced_line);

            fprintf(stderr, "-%s: da_pop error", SHELL_NAME);
            raise(SIGINT);
            return;
        }
    }
}

CharArray *shell_clean_line(const CharArray *line) {
    const int SPACE_REPLACEMENT = -2;
    size_t line_len = da_count_char(line);

    CharArray *cleaned_line = da_new_char();
    da_resize_char(cleaned_line, line_len, '\0');

    // Clean up newlines, backslashes (\), spaces, parens, braces, etc...
    shell_clean_most(cleaned_line, line, SPACE_REPLACEMENT);

    // Add spaces between parts as necessary (after ;, around &&, around ||, around |)
    CharArray *spaced_line = da_new_char();
    shell_clean_syntax(spaced_line, cleaned_line);

    return spaced_line;
}
