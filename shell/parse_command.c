#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "helpers/commands/check.h"
#include "helpers/commands/clean.h"
#include "parse_command.h"

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
