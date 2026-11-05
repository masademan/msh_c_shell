#ifndef SHELL_READ_LINE_H
#define SHELL_READ_LINE_H

#include <stddef.h>
#include <stdbool.h>

/* Reads the command the user types in character by character
   Returns a string (pointer to a char) built in that way
   The caller owns the pointer returned and has to call free() on it to free it */
char *shell_read_line(void);

#endif
