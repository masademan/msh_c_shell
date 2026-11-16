#include <stdio.h>
#include <stdlib.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "macros/shell_globals.h"
#include "shell/helpers/arguments/clean.h"
#include "shell/helpers/commands/check.h"
#include "split.h"


StringArray *shell_token_to_arguments(const String *token) {
    StringArray *args = da_new_string();
    CharArray *temp_arg_arr = da_new_char();
    String *temp_arg_string;
    char c;

    line_check_reset();

    for (size_t i = 0; i < ss_length(token); i++) {
        if (!ss_at(token, i, &c)) {
            da_free_char(temp_arg_arr);
            da_free_string(args);

            fprintf(stderr, "-%s: ss_at error\n", SHELL_NAME);
            return NULL;
        }

        check_next_char(c);

        if (!has_open_quotes() && c == ' ') {
            temp_arg_string = da_join_to_string(temp_arg_arr);

            clean_argument(&temp_arg_string);

            if (!da_add_string(args, temp_arg_string)) {
                ss_free(temp_arg_string);
                da_free_char(temp_arg_arr);
                da_free_string(args);

                fprintf(stderr, "-%s: da_add error\n", SHELL_NAME);
                return NULL;
            }

            ss_free(temp_arg_string);
            da_clear_char(temp_arg_arr);
        } else {
            if (!da_add_char(temp_arg_arr, c)) {
                da_free_char(temp_arg_arr);
                da_free_string(args);

                fprintf(stderr, "-%s: da_add error\n", SHELL_NAME);
                return NULL;
            }
        }
    }

    if (da_count_char(temp_arg_arr) > 0) {
        temp_arg_string = da_join_to_string(temp_arg_arr);

        clean_argument(&temp_arg_string);

        if (!da_add_string(args, temp_arg_string)) {
            ss_free(temp_arg_string);
            da_free_char(temp_arg_arr);
            da_free_string(args);

            fprintf(stderr, "-%s: da_add error\n", SHELL_NAME);
            return NULL;
        }

        ss_free(temp_arg_string);
        da_clear_char(temp_arg_arr);
    }

    da_free_char(temp_arg_arr);

    return args;
}

const char **string_arg_array_to_execvp_arg_array(const StringArray *string_args) {
    if (string_args == NULL) {
        return NULL;
    }

    size_t num_args = da_count_string(string_args);
    const char **char_args = malloc(sizeof(char *) * (num_args + 1));
    if (char_args == NULL) {
        fprintf(stderr, "-%s: couldn't allocate mamoery for command arguments\n", SHELL_NAME);
        return NULL;
    }

    String *string_arg;
    for (size_t i = 0; i < num_args; i++) {
        if (!da_get_string(string_args, i, &string_arg)) {
            free(char_args);

            fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
            return NULL;
        }

        char_args[i] = ss_cstr(string_arg);
    }

    char_args[num_args] = NULL;

    return char_args;
}
