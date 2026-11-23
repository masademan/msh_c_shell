#include "data_type/dynamic_arrays/string_array.h"
#include "history.h"

static StringArray *command_history;

void setup_command_history(void) { command_history = da_new_string(); }

void clean_up_command_history(void) { da_free_string(command_history); }
