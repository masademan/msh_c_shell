#include <stdlib.h>
#include <string.h>

#include "environ_var.h"

char *get_environ_var_val(const char *variable) {
    char *var_val = getenv(variable);

    if (var_val == NULL) {
        return "";
    }
    return var_val;
}

bool set_environ_var_val(const char *variable, const char *value, bool do_overwrite) {
    if (variable == NULL || variable[0] == '\0' || strchr(variable, '=') != NULL) {
        return false;
    }
    if (value == NULL) {
        return false;
    }

    if (setenv(variable, value, do_overwrite) != 0) {
        return false;
    }

    return true;
}
