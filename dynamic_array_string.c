/* dynamic_array.c - your implementation of dynamic_array.h.

   The one rule: do not call realloc. Allocate the bigger block, copy the data across and
   free the old block yourself. The rest of the standard C library is allowed.

   Every function below is a stub. It compiles and returns something harmless, so
   `make m1` runs from the start. Replace each stub's body with your own code. */

#include <stdio.h>
#include <stdlib.h>

#include "dynamic_array_int.h"

/* Define struct IntArray here. */
struct IntArray {
    int *data;
    size_t len;
    size_t alloc_size;
};

IntArray *da_new_int(void) {
    IntArray *da;

    da = malloc(sizeof *da);
    if (da == NULL) {
        return NULL;
    }

    da->data = NULL;
    da->len = 0;
    da->alloc_size = 0;

    return da;
}

void da_free_int(IntArray *a) {
    if (a == NULL) {
        return;
    }

    free(a->data);
    free(a);
}

size_t da_count_int(const IntArray *a) {
    if (a == NULL) {
        return 0;
    }

    return a->len;
}

/* Moves a's data into a new block with room for capacity elements. If capacity is less than the
   total elements in a, then a won't be changed at all and the function will return true.
   Returns false if memory runs out or a is NULL. */
static bool set_size_to(IntArray *a, size_t capacity) {
    if (a == NULL) {
        return false;
    }

    if (capacity <= da_count_int(a)) {
        return true;
    }

    int *new_data;
    new_data = malloc(sizeof(*new_data) * capacity);
    if (new_data == NULL) {
        return false;
    }

    for (size_t i = 0; i < da_count_int(a); i++) {
        new_data[i] = a->data[i];
    }

    free(a->data);
    a->data = new_data;
    a->alloc_size = capacity;

    return true;
}

/* Reduces the allocated size of a when possible. The condition for when to shrink the array
   can be changed. Does nothing is a is NULL. This function does use malloc to get new memory
   to shrink the array. But if memory runs out, the array won't be shrunken and the removing
   functions still work as intended, never failing because of memory running out, keeping its
   promise. */
static void shrink_array_when_small(IntArray *a) {
    if (a == NULL) {
        return;
    }

    // Edit shrinking condition here
    bool shrink_cond = (da_count_int(a) << 2) <= da_alloc_size_int(a);
    if (!shrink_cond) {
        return;
    }

    // Do array shrinking here
    size_t shrunken_size = da_count_int(a) << 1;

    // If the setting size fails, a doesn't change and the code just continues like nothing
    //   happened
    set_size_to(a, shrunken_size);
}

bool da_reserve_int(IntArray *a, size_t n) {
    if (a == NULL) {
        return false;
    }

    if (a->alloc_size >= n) {
        return true;
    }

    return set_size_to(a, n);
}

bool da_add_int(IntArray *a, int value) {
    if (a == NULL) {
        return false;
    }

    if (a->len == a->alloc_size) {
        // Increase alloc size by 1
        /*
        if (!set_size_to(a, da_count(a) + 1)) {
            return false;
        }
        */

        // Multiply alloc size by 2
        /**/
        if (da_count_int(a) == 0) {
            if (!set_size_to(a, 1)) {
                return false;
            }
        } else {
            if (!set_size_to(a, da_count_int(a) << 1)) {
                return false;
            }
        }
        /**/

        // Multiply alloc size by 3
        /*
        if (da_count(a) == 0) {
            if (!set_size_to(a, 1)) {
                return false;
            }
        } else {
            if (!set_size_to(a, da_count(a) + (da_count(a) << 1))) {
                return false;
            }
        }
        */

        // Multiply alloc size by 2.5
        /*
        if (da_count(a) == 0) {
            if (!set_size_to(a, 1)) {
                return false;
            }
        } else {
            if (!set_size_to(a, (da_count(a) >> 1) + (da_count(a) << 1))) {
                return false;
            }
        }
        */
    }

    a->data[a->len] = value;
    a->len++;

    return true;
}

bool da_insert_int(IntArray *a, size_t i, int value) {
    if (a == NULL) {
        return false;
    }
    if (i > da_count_int(a)) {
        return false;
    }

    if (!da_add_int(a, 0)) {
        return false;
    }

    if (da_count_int(a) == 1) {
        a->data[0] = value;
        return true;
    }

    int prev_temp = value;
    int curr_temp = a->data[i];
    for (size_t idx = i; idx < da_count_int(a) - 1; idx++) {
        a->data[idx] = prev_temp;
        prev_temp = curr_temp;
        curr_temp = a->data[idx + 1];
    }
    a->data[da_count_int(a) - 1] = prev_temp;

    return true;
}

bool da_get_int(const IntArray *a, size_t i, int *out) {
    if (a == NULL || out == NULL || i >= da_count_int(a)) {
        return false;
    }

    *out = a->data[i];

    return true;
}

bool da_set_int(IntArray *a, size_t i, int value) {
    if (a == NULL) {
        return false;
    }

    if (i >= da_count_int(a)) {
        return false;
    }

    a->data[i] = value;
    return true;
}

bool da_copy_to_int(const IntArray *a, int *dest, size_t dest_len) {
    if (a == NULL || dest == NULL) {
        return false;
    }

    if (dest_len < da_count_int(a)) {
        return false;
    }

    for (size_t idx = 0; idx < da_count_int(a); idx++) {
        dest[idx] = a->data[idx];
    }

    return true;
}

bool da_index_of_int(const IntArray *a, int value, size_t *index) {
    if (a == NULL || index == NULL) {
        return false;
    }

    for (size_t idx = 0; idx < da_count_int(a); idx++) {
        if (a->data[idx] == value) {
            *index = idx;
            return true;
        }
    }

    return false;
}

bool da_contains_int(const IntArray *a, int value) {
    size_t unused;
    return da_index_of_int(a, value, &unused);
}

bool da_remove_at_int(IntArray *a, size_t i, int *out) {
    if (a == NULL) {
        return false;
    }
    if (i >= da_count_int(a)) {
        return false;
    }

    da_get_int(a, i, out);

    for (size_t idx = i; idx < da_count_int(a) - 1; idx++) {
        a->data[idx] = a->data[idx + 1];
    }

    a->len--;

    shrink_array_when_small(a);

    return true;
}

bool da_remove_int(IntArray *a, int value) {
    size_t idx;

    if (!da_index_of_int(a, value, &idx)) {
        return false;
    }

    da_remove_at_int(a, idx, NULL);
    shrink_array_when_small(a);

    return true;
}

void da_clear_int(IntArray *a) {
    if (a == NULL) {
        return;
    }

    a->len = 0;

    shrink_array_when_small(a);
}

/* My functions */
size_t da_alloc_size_int(const IntArray *a) {
    if (a == NULL) {
        return 0;
    }
    return a->alloc_size;
}

IntArray *da_new_from_array_int(const int *values, size_t n) {
    if (values == NULL) {
        return NULL;
    }

    IntArray *a = da_new_int();

    if (!da_reserve_int(a, n)) {
        da_free_int(a);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        da_add_int(a, values[i]);
    }

    return a;
}

bool da_extend_int(IntArray *a, const IntArray *b) {
    if (a == NULL || b == NULL) {
        return false;
    }

    if (!da_reserve_int(a, da_count_int(a) + da_count_int(b))) {
        return false;
    }

    for (size_t b_idx = 0; b_idx < da_count_int(b); b_idx++) {
        da_add_int(a, b->data[b_idx]);
    }

    return true;
}

IntArray *da_copy_int(const IntArray *a) {
    if (a == NULL) {
        return NULL;
    }

    IntArray *a_copy = da_new_int();

    if (!da_reserve_int(a_copy, da_count_int(a))) {
        da_free_int(a_copy);
        return NULL;
    }

    for (size_t idx = 0; idx < da_count_int(a); idx++) {
        da_add_int(a_copy, a->data[idx]);
    }

    return a_copy;
}

/* ---- Small ---- */
IntArray *da_reverse_int(const IntArray *a) {
    if (a == NULL) {
        return NULL;
    }

    IntArray *reversed_a = da_new_int();

    if (!da_reserve_int(reversed_a, da_count_int(a))) {
        da_free_int(reversed_a);
        return NULL;
    }

    for (size_t idx = 0; idx < da_count_int(a); idx++) {
        da_add_int(reversed_a, a->data[da_count_int(a) - idx - 1]);
    }

    return reversed_a;
}

bool da_pop_int(IntArray *a, int *out) {
    if (a == NULL) {
        return false;
    }
    if (da_count_int(a) == 0) {
        return false;
    }

    da_get_int(a, da_count_int(a) - 1, out);
    a->len--;

    shrink_array_when_small(a);

    return true;
}

bool da_add_all_int(IntArray *a, const int *values, size_t n) {
    if (a == NULL || values == NULL) {
        return false;
    }

    if (!da_reserve_int(a, da_count_int(a) + n)) {
        return false;
    }

    for (size_t idx = 0; idx < n; idx++) {
        da_add_int(a, values[idx]);
    }

    return true;
}

bool da_count_of_int(const IntArray *a, int value, size_t *count) {
    if (a == NULL || count == NULL) {
        return false;
    }

    size_t count_of = 0;

    for (size_t idx = 0; idx < da_count_int(a); idx++) {
        if (a->data[idx] == value) {
            count_of++;
        }
    }

    *count = count_of;

    return true;
}

/* ---- Medium ---- */
bool da_resize_int(IntArray *a, size_t n, int fill) {
    if (a == NULL) {
        return false;
    }

    if (da_count_int(a) == n) {
        return true;
    }

    if (n < da_count_int(a)) {
        // Shrinking a
        a->len = n;
    } else {
        // Growing a
        if (!da_reserve_int(a, n)) {
            return false;
        }

        size_t fills_needed = n - da_count_int(a);
        for (size_t i = 0; i < fills_needed; i++) {
            da_add_int(a, fill);
        }
    }

    shrink_array_when_small(a);

    return true;
}

IntArray *da_slice_steps_int(const IntArray *a, size_t start, size_t end, int steps) {
    if (a == NULL || steps == 0) {
        return NULL;
    }

    IntArray *sliced_array = da_new_int();

    if (da_count_int(a) == 0) {
        return sliced_array;
    }

    size_t true_start = start;
    size_t true_end = end;

    if (start > da_count_int(a) - 1) {
        true_start = da_count_int(a) - 1;
    }
    if (end > da_count_int(a) - 1) {
        true_end = da_count_int(a) - 1;
    }

    if (start > da_count_int(a) - 1 && end > da_count_int(a) - 1) {
        return sliced_array;
    }

    size_t sliced_size = 0;
    if (steps > 0) {
        if (start > end) {
            return sliced_array;
        }
        sliced_size = (true_end - true_start) / steps;
    } else {
        if (start < end) {
            return sliced_array;
        }
        sliced_size = (true_start - true_end) / (-steps);
    }

    sliced_size++;

    if (!da_reserve_int(sliced_array, sliced_size)) {
        da_free_int(sliced_array);
        return NULL;
    }

    for (size_t i = 0; i < sliced_size; i++) {
        da_add_int(sliced_array, a->data[true_start + i * steps]);
    }

    return sliced_array;
}

IntArray *da_slice_int(const IntArray *a, size_t start, size_t end) {
    if (a == NULL) {
        return NULL;
    }

    return da_slice_steps_int(a, start, end, 1);
}

bool da_remove_all_int(IntArray *a, int value) {
    if (a == NULL) {
        return false;
    }

    size_t offset = 0;
    for (size_t idx = 0; idx + offset < da_count_int(a); idx++) {
        while (idx + offset < da_count_int(a) && a->data[idx + offset] == value) {
            offset++;
        }

        if (idx + offset < da_count_int(a)) {
            a->data[idx] = a->data[idx + offset];
        }
    }

    a->len -= offset;

    shrink_array_when_small(a);

    return true;
}

/* ---- Big ---- */
static int ascending_compare(const void *a, const void *b) {
    int a_int = *(const int *) a;
    int b_int = *(const int *) b;

    if (a_int < b_int) {
        return -1;
    }
    if (a_int > b_int) {
        return 1;
    }

    return 0;
}

static int descending_compare(const void *a, const void *b) {
    int a_int = *(const int *) a;
    int b_int = *(const int *) b;

    if (a_int < b_int) {
        return 1;
    }
    if (a_int > b_int) {
        return -1;
    }

    return 0;
}

bool da_sort_int(IntArray *a, bool ascending_order) {
    if (a == NULL) {
        return false;
    }
    if (a->len == 0) {
        return true;
    }

    if (ascending_order) {
        qsort(a->data, a->len, sizeof(int), ascending_compare);
    } else {
        qsort(a->data, a->len, sizeof(int), descending_compare);
    }

    return true;
}

bool da_binary_search_int(const IntArray *a, int value, size_t *out) {
    if (a == NULL) {
        return false;
    }
    if (da_count_int(a) == 0) {
        return false;
    }

    size_t left = 0;
    size_t right = da_count_int(a) - 1;
    size_t mid;

    while (left <= right) {
        mid = (left + right) >> 1;

        if (a->data[mid] == value) {
            if (out != NULL) {
                *out = mid;
            }

            return true;
        } else if (a->data[mid] < value) {
            left = mid + 1;
        } else if (a->data[mid] > value) {
            if (mid == 0) {
                return false;
            }

            right = mid - 1;
        }
    }

    return false;
}


/* --- Shell added --- */
void da_remove_n_int(IntArray *a, size_t i, size_t remove_count) {
    if (a == NULL) {
        return;
    }
    if (remove_count == 0) {
        return;
    }
    if (i + remove_count >=
        da_count_int(a) + 1) {  // almost equivalent to i + remove_count - 1 >= da_count_int(a)
        return;
    }

    for (size_t idx = i + remove_count; idx < da_count_int(a); idx++) {
        a->data[idx - remove_count] = a->data[idx];
    }

    a->len -= remove_count;

    shrink_array_when_small(a);
}
