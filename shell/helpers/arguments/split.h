#ifndef SPLIT_ARGUMENTS_H
#define SPLIT_ARGUMENTS_H

#include "data_type/dynamic_arrays/string_array.h"

/* Takes in a single token (command + its args) and splits
   it into an array of strings
   The first string is the command, and the following strings (if there are any)
   are the arguments for that command */
StringArray *shell_token_to_arguments(const String *token);

/* Takes an array of Strings and outputs an array of strings (array of chars) that's
   NULL terminated
   This is to make freeing much easier (just free the pointer returned, then free the StringArray later)
   The caller owns the returned pointer and has to call free() on it to free it */
const char **string_arg_array_to_execvp_arg_array(const StringArray *string_args);

#endif
