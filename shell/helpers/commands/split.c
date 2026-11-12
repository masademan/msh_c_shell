#include "data_type/dynamic_arrays/char_array.h"
#include "split.h"

StringArray *shell_split_line(CharArray *line) {
    StringArray *arguments = da_new_string();
    CharArray *temp_argument = da_new_char();
    char c;

    for (size_t i = 0; i < da_count_char(line); i++) {
    }

    (void) line;
    (void) arguments;
    (void) temp_argument;
    (void) c;

    return NULL;
}
