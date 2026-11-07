#include <stddef.h>

#include "shell_parse_line.h"

// TODO: Make a "clean_line" function to remove '\' from the line to make parsing easier
//         This will also allow for more complex line-breaking rules in the reading stage

/* Cleans the incoming line from shell_read_line
   It would:
    Replace '\' with ' '
    Replace '\n' with ' '
    And do more
   This is meant to make parsing easier, as well as allow for multi-line commands
   */
static char *shell_clean_line(char *line) {
    (void) line;

    return NULL;
}

/* This splits the line that comes from shell_clean_line and returns a
    dynamic array of pointers (pointer to a pointer to a char)
    This is what is read to execute commands and run programs */
static char **shell_split_line(char *line) {
    (void) line;

    return NULL;
}

char **shell_parse_line(char *line) {
    (void) line;

    shell_clean_line(NULL);
    shell_split_line(NULL);

    return NULL;
}

void arg_free(char **args) { (void) args; }
