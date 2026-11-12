#include <signal.h>
#include <stddef.h>
#include <stdio.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "helpers/commands/clean.h"
#include "helpers/commands/split.h"
#include "macros/shell_globals.h"
#include "parse_command.h"

StringArray *shell_parse_line(CharArray *line) {
    CharArray *cleaned_line = shell_clean_line(line);
    StringArray *split_args = shell_split_line(cleaned_line);

    String *s_cleaned_line = da_join_to_string(cleaned_line);
    printf("\n\"\"\"\n%s\n\"\"\"\n\n", ss_cstr(s_cleaned_line));
    ss_free(s_cleaned_line);

    String *s_argument;
    for (size_t arg_i = 0; arg_i < da_count_string(split_args); arg_i++) {
        if (!da_get_string(split_args, arg_i, &s_argument)) {
            da_free_char(cleaned_line);

            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            raise(SIGINT);
            da_free_string(split_args);
            return NULL;
        }

        printf("arg %zu: ⟨%s⟩\n", arg_i + 1, ss_cstr(s_argument));
    }

    da_free_char(cleaned_line);
    return split_args;
}
