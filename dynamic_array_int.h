#ifndef DYNAMIC_ARRAY_INT_H
#define DYNAMIC_ARRAY_INT_H

#include <stdbool.h>
#include <stddef.h>

/* An ordered array of ints that grows as you add to it. */
typedef struct IntArray IntArray;

/* ---- Making and freeing ---- */

/* A new, empty IntArray. NULL if memory runs out.
   Whoever gets one must pass it to da_free exactly once. */
IntArray *da_new_int(void);

/* Frees a and everything it holds. Does nothing if a is NULL. */
void da_free_int(IntArray *a);

/* ---- Size ---- */

/* The number of ints in a. 0 if a is NULL. */
size_t da_count_int(const IntArray *a);

/* A hint that a will soon hold at least n ints, so it can make room now. Returns true
   if a can now hold n ints without allocating again. Returns false, and leaves a
   unchanged, if a is NULL or memory runs out. */
bool da_reserve_int(IntArray *a, size_t n);

/* ---- Adding ---- */

/* Adds value to the end of a and returns true. Returns false, and leaves a unchanged,
   if a is NULL or memory runs out. */
bool da_add_int(IntArray *a, int value);

/* Inserts value at index i, shifting the ints at i and after up one index, and returns
   true. i may equal da_count_int(a), which adds to the end. Returns false, and leaves a
   unchanged, if a is NULL, i > da_count_int(a), or memory runs out. */
bool da_insert_int(IntArray *a, size_t i, int value);

/* ---- Reading and writing ---- */

/* Stores the int at index i in *out and returns true. Returns false, and leaves *out
   alone, if a or out is NULL or i is not less than da_count_int(a). */
bool da_get_int(const IntArray *a, size_t i, int *out);

/* Replaces the int at index i with value and returns true. Returns false, and leaves a
   unchanged, if a is NULL or i is not less than da_count_int(a). */
bool da_set_int(IntArray *a, size_t i, int value);

/* Copies a's ints, in order, into dest[0 .. da_count_int(a) - 1] and returns true.
   Returns false, and writes nothing, if a or dest is NULL or dest_len < da_count_int(a). */
bool da_copy_to_int(const IntArray *a, int *dest, size_t dest_len);

/* ---- Searching ---- */

/* Finds the first index holding value: stores it in *index and returns true. Returns
   false, and leaves *index alone, if value does not occur or a or index is NULL. */
bool da_index_of_int(const IntArray *a, int value, size_t *index);

/* true if value occurs in a. false if a is NULL. */
bool da_contains_int(const IntArray *a, int value);

/* ---- Removing ----

   Removing never needs new memory, so these cannot fail for lack of it. */

/* Removes the int at index i, shifting later ints down one index, and returns true.
   If out is not NULL, stores the removed int in *out first. Returns false, and leaves
   a and *out unchanged, if a is NULL or i is not less than da_count_int(a). */
bool da_remove_at_int(IntArray *a, size_t i, int *out);

/* Removes the first occurrence of value from a, shifting later ints down one index,
   and returns true. Returns false if value does not occur or a is NULL. */
bool da_remove_int(IntArray *a, int value);

/* Removes every int from a, so its count is 0. Does nothing if a is NULL. */
void da_clear_int(IntArray *a);

/* ---- Your additions ----

   Declare your extension functions below this line, each with a comment in the style of
   the ones above: what it returns, what it does with NULL, and who owns the result.
   Do not change anything above this line. */

/* The amount of allocated memory for a. 0 if a is NULL. */
size_t da_alloc_size_int(const IntArray *a);

/* Returns a new array that copies the n items from values.
   Returns NULL if values is NULL or memory runs out.
   Caller owns the outputted array and has to use da_free to free it. */
IntArray *da_new_from_array_int(const int *values, size_t n);

/* Appends all the values from b onto the end of a. Only a is changed, b stays constant.
   Returns true if successful. Returns false, if a or b is NULL, or memory runs out. */
bool da_extend_int(IntArray *a, const IntArray *b);

/* Returns a new array that copies all of a's values, but has a different pointer.
   Returns NULL, if a is NULL or memory runs out. Caller owns the returned array and has to pass
   it to da_free to free the memory used. */
IntArray *da_copy_int(const IntArray *a);

/* ---- Small ---- */
/* Returns a new array that's identical to a except that all the elements are arranged
   in the opposite order. Returns NULL when a is NULL or memory runs out. The caller owns
   the returned array and has to pass it to da_free to free the memory used. */
IntArray *da_reverse_int(const IntArray *a);

/* Removes the last element in the array and sets *out to the removed value. Returns true if the
   value was successfully removed. If out is NULL, then no value will be set.
   Returns false and leaves a and *out alone, if a is NULL, or a is empty. */
bool da_pop_int(IntArray *a, int *out);

/* Adds n ints from values to the end of array a, allocating enough space for the new array once.
   Returns true if this worked successfully. Returns false and leaves a unchanged,
   if a or values is NULL, or memory runs out. */
bool da_add_all_int(IntArray *a, const int *values, size_t n);

/* Counts all the instances of value in a and sets count to that number. Returns false and
   leaves count alone, if a or count is NULL. Otherwise returns true. */
bool da_count_of_int(const IntArray *a, int value, size_t *count);

/* ---- Medium ---- */
/* Sets the size of a to n, dropping elements from the end if shrinking the array, or adding
   copies of fill to the end if growing the array. Returns true if successful. Returns false
   and leaves a unchanged, if a is NULL or memory runs out. */
bool da_resize_int(IntArray *a, size_t n, int fill);

/* Creates a new IntArray with every steps elements from a, starting at index start and ending
   at index end. Returns NULL, if a is NULL, memory runs out, or steps is 0. If the range between start
   and end is out of bounds, da_slice_steps will return an empty array. If steps is negative, start
   should be greater than or equal to end, and if steps is positive, then end should be greater than or
   equal to start, otherwise you get an empty array. Caller owns the outputted array and has to use
   da_free to free it. */
IntArray *da_slice_steps_int(const IntArray *a, size_t start, size_t end, int steps);

/* Creates a new IntArray with every element from a, starting at index start and ending
   at index end. Returns NULL, if a is NULL or memory runs out.
   If the range between start and end is out of bounds, da_slice will return an empty array.
   Caller owns the outputted array and has to use da_free to free it. */
IntArray *da_slice_int(const IntArray *a, size_t start, size_t end);

/* Removes all instances of value in a and shifts all the later items to fill in the gaps.
   Returns false, if a is NULL. Otherwise returns true. */
bool da_remove_all_int(IntArray *a, int value);

/* ---- Big ---- */
/* Sorts the inputted array and returns true if successful. ascending_order controls whether or not
   the array will be sorted in ascending order or not.
   Returns false and leaves a unchanged, if a is NULL. */
bool da_sort_int(IntArray *a, bool ascending_order);

/* Searches through the inputted array and outputs true if the value is found, otherwise false.
   The inputted array must be sorted in ascending order for the binary search to work properly.
   The index that the value is found at is outputted through *out.
   If out is NULL, then the value won't be set.
   Returns false, if a is NULL or the value isn't found */
bool da_binary_search_int(const IntArray *a, int value, size_t *out);

/* shrink_array_when_small */
/* Reduces the allocated size of a when possible. The condition for when to shrink the array
   can be changed. Does nothing is a is NULL. This function does use malloc to get new memory
   to shrink the array. But if memory runs out, the array won't be shrunken and the removing
   functions still work as intended, never failing because of memory running out, keeping its
   promise. */


/* --- Shell added --- */

/* Removes n ints from a starting at i and moving right, and shifts the later ints
   to fill in the gaps.
   Does nothing if a is NULL, remove_count is 0,
   or i + remove_count - 1 goes past the end of the array */
void da_remove_n_int(IntArray *a, size_t i, size_t remove_count);

/* Removes consecutive duplicates of val
   Returns false if a is NULL
   Else, returns true */
bool da_remove_duplicate_int(IntArray *a, int val);

/* Replaces all instances of to_replace with replace_with
   Returns false if a is NULL
   Else, return true */
bool da_replace_int(IntArray *a, int to_replace, int replace_with);

#endif
