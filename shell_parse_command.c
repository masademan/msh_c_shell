#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "char_array_and_string_conversion.h"
#include "dynamic_array_char.h"
#include "dynamic_array_string.h"
#include "safe_string.h"
#include "shell_parse_command.h"

// TODO: Make a "clean_line" function to remove '\' from the line to make parsing easier
//         This will also allow for more complex line-breaking rules in the reading stage

/* Cleans the incoming line from shell_read_line
   It would:
    Replace "\\" with '\'
    Replace "\\\n" with ""
    And do more
   This is meant to make parsing easier, as well as allow for multi-line commands
   This function returns a new pointer that allocates memory
   The caller owns the pointer returned and has to use free() on it to free it */
static CharArray *shell_clean_line(CharArray *line) {
    String *s_line = da_join_to_string(line);
    printf("\n\"\"\"\n%s\n\"\"\"\n", ss_cstr(s_line));
    ss_free(s_line);

    return NULL;
}

/* This splits the line that comes from shell_clean_line and returns a
    dynamic array of pointers (pointer to a pointer to a char)
    This is what is read to execute commands and run programs
    This function returns a new pointer that allocates memory for even more
    pointers that allocate memory
    The caller owns the pointer returned, as well as each subpointer,
    and has to call arg_free() on it to free it */
static StringArray *shell_split_line(CharArray *line) {
    (void) line;

    return NULL;
}

StringArray *shell_parse_line(CharArray *line) {
    CharArray *cleaned_line = shell_clean_line(line);
    StringArray *split_args = shell_split_line(cleaned_line);

    String *s_cleaned_line = da_join_to_string(cleaned_line);
    printf("\n\"\"\"\n%s\n\"\"\"\n", ss_cstr(s_cleaned_line));
    ss_free(s_cleaned_line);

    da_free_char(cleaned_line);
    return split_args;
}
