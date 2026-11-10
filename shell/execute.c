#include <stddef.h>

#include "execute.h"

// Built-in function prototypes
// Example:
//   int shell_cd(StringArray *args);
int shell_cd(StringArray *args);

// Built-in setup
static char *built_in_command_names[] = {
    "cd",
    "exit",
    "help",
};

static int (*built_in_command_funcs[])(StringArray *) = {
    &shell_cd,
};

// Make a list for the built-in commands in the same order as the built_in_commands array

static size_t num_built_ins(void) { return sizeof(built_in_command_names) / sizeof(char *); }

// Built-in functions
// Copy of the prototypes, but finished
int shell_cd(StringArray *args) {
    // TODO: finish implementation
    (void) args;

    return 1;
}

// Global functions
// make it so that ctrl + c kills the child process, end the execution in the parent, and return 1
//  should bring user back to shell
int shell_execute(StringArray *args) {
    (void) args;
    (void) built_in_command_funcs;
    num_built_ins();

    return 1;
}

int shell_launch_program(StringArray *args) {
    (void) args;

    return 0;
}

int run_shell_command(StringArray *args) {
    (void) args;

    return 0;
}
