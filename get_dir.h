#ifndef GET_DIR_H
#define GET_DIR_H

#include <stddef.h>
#include <stdbool.h>

/* Returns a string (pointer to chars) that gives the current working directory of the program
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_dir(void);

#endif
