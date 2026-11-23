#include <stdio.h>

#include "export.h"

bool parse_export_arg(const char *export_arg, String **environ_var_name, String **environ_var_val) {
    (void) export_arg;
    (void) environ_var_name;
    (void) environ_var_val;

    printf("%s\n", export_arg);

    return false;
}
