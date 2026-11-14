#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "execute.h"
#include "helpers/commands/check.h"
#include "macros/shell_globals.h"

// Built-in function prototypes
// Example:
//   int shell_cd(const StringArray *args);
static int shell_cd(const StringArray *args);

// Built-in setup
static char *built_in_command_names[] = {
    "cd",
    "exit",
    "help",
};

static int (*built_in_command_funcs[])(const StringArray *) = {
    &shell_cd,
};

// Make a list for the built-in commands in the same order as the built_in_commands array

static size_t num_built_ins(void) { return sizeof(built_in_command_names) / sizeof(char *); }

// Built-in functions
// Copy of the prototypes, but finished
static int shell_cd(const StringArray *args) {
    // TODO: finish implementation
    (void) args;

    return 1;
}

// Static functions
/* This is to find where the next token is
   Usually the offset is 1 (very next token), but if there are
   parentheses, the offset is higher since those part are handled with forks,
   which have to merge back with the parent process */
static size_t offset_to_next_token(const StringArray *tokens, size_t curr_idx) {
    (void) tokens;
    (void) curr_idx;

    line_check_reset();

    return 0;
}

// Global functions
// make it so that ctrl + c kills the child process, end the execution in the parent, and return 1
//  should bring user back to shell
int shell_execute(const StringArray *tokens) {
    (void) tokens;
    (void) built_in_command_names;
    (void) built_in_command_funcs;
    num_built_ins();
    offset_to_next_token(tokens, 0);

    printf("Tokens:\n");
    String *s_token;
    for (size_t token_i = 0; token_i < da_count_string(tokens); token_i++) {
        if (!da_get_string(tokens, token_i, &s_token)) {
            fprintf(stderr, "-%s: da_get error", SHELL_NAME);
            return 0;
        }

        printf(" - token %zu: ⟨%s⟩\n", token_i + 1, ss_cstr(s_token));
    }

    return 1;
}

// make another function that takes in an arg and splits it into
//   more args that can be run
// this function would be called in another function that goes
//   through the arguments given and does forks if necessary

StringArray *shell_token_to_arguments(const String *token) {
    (void) token;

    return NULL;
}

int shell_execute_command(const StringArray *args) {
    (void) args;

    return 0;
}

int shell_launch_program(const StringArray *args) {
    (void) args;

    return 0;
}

int run_shell_command(const StringArray *args) {
    (void) args;

    return 0;
}
