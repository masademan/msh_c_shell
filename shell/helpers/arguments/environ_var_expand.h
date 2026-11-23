#ifndef ENVIRON_VAR_EXPAND_H
#define ENVIRON_VAR_EXPAND_H

#include <stdbool.h>

#include "data_type/dynamic_arrays/char_array.h"

/* Pulls the expanding of the environment value and inserting into the array into it's own function */
bool expand_the_environ_var_val(CharArray *temp_environ_var, CharArray *expanded_arg);

/* Takes in a clean argument (from clean_argument) and expands any environment variables necessary
   Caller owns the returns pointer and has to call da_free_char() on it to free it */
CharArray *expand_environ_vars(const CharArray *cleaned_arg);

#endif
