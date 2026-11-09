#ifndef SHELL_READ_COMMAND_H
#define SHELL_READ_COMMAND_H

#include "dynamic_array_char.h"

/* Reads the single line command the user types in character by character
   Returns a string (pointer to a char) built in that way
   The caller owns the pointer returned and has to call free() on it to free it
 */
CharArray *shell_read_line(void);

/* Reads the multiline or single line command the user types
   Uses shell_read_line to read each line individually,
   and glues it together as it goes
   The caller owns the pointer returned and has to call free() on it to free it
 */
CharArray *shell_read_command(void);

#endif
