#ifndef EXPORT_H
#define EXPORT_H

#include <stdbool.h>

#include "data_type/safe_string/safe_string.h"

/* Takes a string in from the export command, and sets environ_var_name and _val to their respecitve
   strings (the name and value)
   Returns true if successful
   Returns false if export_arg, environ_var_name, or environ_var_val is NULL, or memory runs out */
bool parse_export_arg(const char *export_arg, String **environ_var_name, String **environ_var_val);

#endif
