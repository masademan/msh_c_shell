#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
static char *shell_clean_line(char *line) {
    printf("\n\"\"\"\n%s\n\"\"\"\n", line);

    return NULL;
}

/* This splits the line that comes from shell_clean_line and returns a
    dynamic array of pointers (pointer to a pointer to a char)
    This is what is read to execute commands and run programs
    This function returns a new pointer that allocates memory for even more
    pointers that allocate memory
    The caller owns the pointer returned, as well as each subpointer,
    and has to call arg_free() on it to free it */
static char **shell_split_line(char *line) {
    (void) line;

    return NULL;
}

char **shell_parse_line(char *line) {
    char *cleaned_line = shell_clean_line(line);
    char **split_args = shell_split_line(cleaned_line);

    printf("\n\"\"\"\n%s\n\"\"\"\n", cleaned_line);

    free(cleaned_line);
    return split_args;
}

// Maybe args can have a '\0' at the end of the list to know when it ends without having a length?
void arg_free(char **args) {
    if (args == NULL) {
        return;
    }
}
