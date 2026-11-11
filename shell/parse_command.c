#include <stddef.h>
#include <stdio.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "helpers/commands/clean.h"
#include "helpers/commands/split.h"
#include "parse_command.h"

StringArray *shell_parse_line(CharArray *line) {
    CharArray *cleaned_line = shell_clean_line(line);
    StringArray *split_args = shell_split_line(cleaned_line);

    String *s_cleaned_line = da_join_to_string(cleaned_line);
    printf("\n\"\"\"\n%s\n\"\"\"\n", ss_cstr(s_cleaned_line));
    ss_free(s_cleaned_line);

    da_free_char(cleaned_line);
    return split_args;
}
