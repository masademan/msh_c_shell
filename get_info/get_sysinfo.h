#ifndef GET_SYSINFO_H
#define GET_SYSINFO_H

#include <stdbool.h>

/* Returns a string (pointer to chars) that gives the username of the active
   user running the program
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_username(void);

/* Returns a string (pointer to chars) that gives the name of the machine
   running the program
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_hostname(void);

/* Returns a string (pointer to chars) that contains the current working directory
   of the program
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_cwd(void);

/* If the cwd starts with the home dir (home_dir), returns true
   Else, returns false */
bool is_in_home(const char *cwd, const char *home_dir);

/* If the cwd starts with the home dir (home_dir), then it replaces that
   beginning home dir part with the shortened_home piece
   No pointer is returned, but the pointer holding cwd will
   If memory runs out, cwd won't be changed */
void shorten_cwd(char **cwd, const char *home_dir, const char *shortened_home);

/* Returns a string (pointer to chars) that contains the current working directory
   of the program
   It also shortens the beginning of the directory to a ~ if it's the same as the path
   at $HOME
   The caller owns the pointer returned and has to call free() on it to free it */
char *get_shortened_cwd(void);

#endif
