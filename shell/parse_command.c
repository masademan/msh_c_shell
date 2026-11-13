#include <stddef.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "helpers/commands/clean.h"
#include "helpers/commands/split.h"
#include "parse_command.h"

StringArray *shell_parse_line(CharArray *line) {
    CharArray *cleaned_line = shell_clean_line(line);
    StringArray *split_tokens = shell_split_line(cleaned_line);

    da_free_char(cleaned_line);

    return split_tokens;
}
