#ifndef GET_DIR_H
#define GET_DIR_H

#include <stdbool.h>
#include <stddef.h>

/* Returns a string (pointer to chars) that gives the username of the active
   user running the program
   The caller owns the pointer returned and has to call free() on it to free it
 */
char *get_username(void);

/* Returns a string (pointer to chars) that gives the name of the machine
   running the program
   The caller owns the pointer returned and has to call free() on it to free it
 */
char *get_hostname(void);

/* Returns a string (pointer to chars) that gives the current working directory
   of the program
   The caller owns the pointer returned and has to call free() on it to free it
 */
char *get_cwd(void);

#endif
