#ifndef SPLIT_COMMAND_H
#define SPLIT_COMMAND_H

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/string_array.h"

/* This splits the line that comes from shell_clean_line and returns a
    dynamic array of Strings
    This is what is read to execute commands and run programs
    This function returns a new pointer that allocates memory
    The caller owns the pointer returned, as well as each subpointer,
    and has to call arg_free() on it to free it */
StringArray *shell_split_line(CharArray *line);

#endif
