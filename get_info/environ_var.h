#ifndef ENVIRONMENT_VAR_H
#define ENVIRONMENT_VAR_H

#include <stdbool.h>

/* Gets the environment variable named variable and returns it */
char *get_environ_var_val(const char *variable);

/* Sets the environment variable named variable to the value named value
   If the environment variable already exists, it gets updated if do_overwrite is true,
   otherwise, it leaves the environment variable alone
   Returns true if the operation was successful
   Returns false if variable is NULL, empty, or contains an equals sign (=); value is NULL; or when out
   of memory */
bool set_environ_var_val(const char *variable, const char *value, bool do_overwrite);

#endif
