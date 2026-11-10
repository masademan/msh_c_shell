#ifndef CHAR_ARRAY_AND_STRING_CONVERSION_H
#define CHAR_ARRAY_AND_STRING_CONVERSION_H

#include "char_array.h"
#include "data_type/safe_string/safe_string.h"

/* Joins a DynamicArray of chars into a single string
   Useful for printing and string manipulations
   Caller owns the returned String and has to call ss_free() on it to free it */
String *da_join_to_string(const CharArray *da);

/* Splits a String into a DynamicArray of chars
   Useful for using array operations on a string
   Caller owns the returned DynamicArray and has to call da_free_char() on it to free it */
CharArray *ss_split_to_char_array(const String *ss);

#endif
