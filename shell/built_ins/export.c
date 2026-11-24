#include <stdio.h>
#include <string.h>

#include "data_type/safe_string/safe_string.h"
#include "export.h"
#include "shell/helpers/arguments/clean.h"

bool parse_export_arg(const char *export_arg, String **environ_var_name, String **environ_var_val) {
    (void) export_arg;
    (void) environ_var_name;
    (void) environ_var_val;

    size_t var_name_len = 0, export_arg_len = strlen(export_arg);
    for (size_t i = 0; i < export_arg_len && export_arg[i] != '='; i++) {
        var_name_len++;
    }

    if (var_name_len == 0) {
        return false;
    }

    String *var_name = ss_new_length(var_name_len);
    String *var_val = ss_new_length(export_arg_len - var_name_len - 1);

    for (size_t i = 0; i < var_name_len; i++) {
        us_set_at(var_name, i, export_arg[i]);
    }
    for (size_t i = var_name_len + 1; i < export_arg_len; i++) {
        us_set_at(var_val, i - var_name_len - 1, export_arg[i]);
    }

    clean_argument(&var_val);

    *environ_var_name = var_name;
    *environ_var_val = var_val;

    return true;
}
