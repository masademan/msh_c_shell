#ifndef CLEAN_ARGUMENT_H
#define CLEAN_ARGUMENT_H

#include "data_type/safe_string/safe_string.h"

/* Takes in a pointer to a String and replaces that with a cleaned String
   This drops quotes at the beginning and end if it exists (not necessary),
   handles ANSI code, escape codes, escape characters, etc.
   The caller only has to call ss_free() on the String that was inputted, as usual */
void clean_argument(String **arg_string);

#endif
