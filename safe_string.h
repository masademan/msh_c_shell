#ifndef SAFE_STRING_H
#define SAFE_STRING_H

#include <stdbool.h>
#include <stddef.h>

/* An immutable string */
typedef struct String String;

/* ---- Making and freeing ----

   Every function in this section except ss_free returns a new String.
   Whoever gets a String from one of these must pass it to ss_free exactly once. */

/* A new String holding a copy of text. NULL if text is NULL or memory runs out. */
String *ss_new(const char *text);

/* A new String with the same bytes as s, in memory of its own.
   NULL if s is NULL or memory runs out. */
String *ss_copy(const String *s);

/* A new String: a's bytes followed by b's. NULL if either is NULL or memory runs out. */
String *ss_concat(const String *a, const String *b);

/* A new String holding len bytes of s, starting at index start.
   NULL if s is NULL, if the range does not fit inside s, or if memory runs out.
   An empty range that fits, such as start == ss_length(s) with len 0, gives "". */
String *ss_substring(const String *s, size_t start, size_t len);

/* A new String with every 'a'..'z' changed to 'A'..'Z' and every other byte kept.
   s itself is unchanged. NULL if s is NULL or memory runs out. */
String *ss_to_upper(const String *s);

/* Frees s and its bytes. Does nothing if s is NULL. */
void ss_free(String *s);

/* ---- Reading ---- */

/* The number of bytes in s, not counting the terminator. 0 if s is NULL. */
size_t ss_length(const String *s);

/* Stores s's byte at index i in *out and returns true. Returns false, and leaves *out
   alone, if s or out is NULL or i is not less than ss_length(s). */
bool ss_at(const String *s, size_t i, char *out);

/* s's bytes as a C string, for printf("%s"). Do not write through it or free it; it stays
   valid until s is freed. "" if s is NULL. */
const char *ss_cstr(const String *s);

/* ---- Comparing and searching ---- */

/* true if a and b hold the same bytes. Two NULLs are equal; NULL and a String are not. */
bool ss_equals(const String *a, const String *b);

/* Negative, zero or positive as a sorts before, equal to, or after b, comparing bytes as
   unsigned char, like strcmp. A String sorts after every String it begins with ("Dun"
   before "Dune"). NULL sorts before any String, and two NULLs compare equal. */
int ss_compare(const String *a, const String *b);

/* true if s begins with prefix. Every String begins with "". false if either is NULL. */
bool ss_starts_with(const String *s, const String *prefix);

/* Finds the first place needle occurs in haystack: stores its index in *index and returns
   true. Returns false, and leaves *index alone, if needle does not occur or any argument
   is NULL. An empty needle is found at index 0. */
bool ss_find(const String *haystack, const String *needle, size_t *index);

/* ---- Your additions ----

   Declare your extension functions below this line, each with a comment in the style of
   the ones above: what it returns, what it does with NULL, and who owns the result.
   Do not change anything above this line. */

/* A new String that only holds a specific length. NULL if memory runs out.
   Bytes are not set except for the terminator at the end. The caller owns
   the new String and must use ss_free to free it. */
String *ss_new_length(size_t len);

/* A new String with every 'A'..'Z' changed to 'a'..'z' and every other byte kept.
   s itself is unchanged. NULL if s is NULL or memory runs out. The caller owns the
   returned String that's returned and has to be freed with ss_free. */
String *ss_to_lower(const String *s);

/* A new String where the order of the chars are reversed.
   s itself is unchanged. NULL if s is NULL or memory runs out. The caller
   owns the String that's returns and has to be freed with ss_free. */
String *ss_reverse(const String *s);

/* Negative, zero or positive as a sorts before, equal to, or after b, comparing bytes as
   unsigned char, like strcmp, except that this function doesn't care about case.
   "Dune", "dUnE", and "dune" all sort after "dUn", "DUN", and "DuN", no matter the case.
   A String sorts after every String it begins with ("Dun" before "Dune"). NULL sorts before
   any String, and two NULLs compare equal. */
int ss_compare_ignore_case(const String *a, const String *b);

/* A new String: all the Strings in str_arr glued together with String separator in between
   all of them. None of the Strings in str_arr are changed.
   NULL if str_arr or separator is NULL, memory runs out, or if any of the Strings in the array
   are NULL. If the array is of length 0, then the function returns an empty String. The caller
   owns the new String and has to pass it to ss_free, to free it. And the caller still owns
   the String array, it's contents, and the separator String. */
String *ss_join(const String **str_arr, size_t str_arr_len, const String *separator);

/* An array of new Strings which are made up of pieces from String s separated at
   String separator, and the number of splits made is stored in *splits.
   String s is unchanged. Returns NULL and leaves *splits alone if any are NULL
   or memory runs out. Returns an array of length 1 if the separator
   can't fit in the input String. The caller owns the returned array and all the Strings
   in it. For the array to be freed, it needs to be passed into the ss_free_arr along with the
   number of splits; ss_free_arr(result, *splits) */
String **ss_split(const String *s, const String *separator, size_t *splits);

/* A new String where all instances of String to_replace in String s are replaced
   with String replace_with. s itself is unchanged. NULL if s, to_replace, or replace_with
   are NULL, or if memory runs out. The caller owns the returned String and has to pass it into
   ss_free to free it. When to_replace is empty, it inserts the replace_with String before
   every byte and at the end. */
String *ss_replace(const String *s, const String *to_replace, const String *replace_with);

/* Frees the pointer to pointers to Strings, which acts as an array. Frees the String
   pointer at index 0 to arr_len - 1, and then frees the s_arr pointer. Does nothing
   if s_arr is NULL. And if arr_len is 0, it'll only free the s_arr. */
void ss_free_arr(String **s_arr, size_t arr_len);

#endif
