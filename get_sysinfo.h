#ifndef GET_DIR_H
#define GET_DIR_H

#include <stddef.h>
#include <stdbool.h>

/* Takes in a size_t size and increases it. Could be doubled or increased by a constant amount.
   This function will be run in get_dir when the directory is too big to fit in the allocated
   memory. Then the output of this function will be the new amount of memory allocated
   If the function returns a number smaller than the inputted size, then the loop will go on
   infinitely, so make sure the function can only increase the value (not counting overflows) */
size_t size_increase_func(size_t size);

/* Returns a string (pointer to chars) that gives the active username of the user running
   the program
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_username(void);

/* Returns a string (pointer to chars) that gives the name of the machine running the program
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_hostname(void);

/* Returns a string (pointer to chars) that gives the current working directory of the program
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_cwd(void);

#endif
