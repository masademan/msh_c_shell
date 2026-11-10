#ifndef SHELL_PARSE_COMMAND_H
#define SHELL_PARSE_COMMAND_H

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/string_array.h"

/* Takes in a string from the shell_read_line function and parses it into
   individual tokens This returns a dynamic array of string (pointer to a
   pointer to a char) which can be inputted into the shell_execute function
   The caller owns the pointer returned and has to call arg_free() on it to free it
 */
StringArray *shell_parse_line(CharArray *line);

#endif
