#ifndef SHELL_CLEAN_COMMAND_H
#define SHELL_CLEAN_COMMAND_H

#include "data_type/dynamic_arrays/char_array.h"

/* Cleans the incoming line from shell_read_command
   It would:
    Replace "\\" with '\'
    Replace "\\\n" with ""
    And do more
   This is meant to make parsing easier, as well as allow for multi-line commands
   This function returns a new pointer that allocates memory
   The caller owns the pointer returned and has to use free() on it to free it */
CharArray *shell_clean_line(const CharArray *line);

#endif
