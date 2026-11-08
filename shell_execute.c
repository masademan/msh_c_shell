#include <stddef.h>

#include "shell_execute.h"

// Built-in function prototypes
// Example:
//   int shell_cd(char **args);
int shell_cd(char **args);

// Built-in setup
static char *built_in_command_names[] = {
    "cd",
    "exit",
    "help",
};

static int (*built_in_command_funcs[])(char **) = {
    &shell_cd,
};

// Make a list for the built-in commands in the same order as the built_in_commands array

static size_t num_built_ins(void) { return sizeof(built_in_command_names) / sizeof(char *); }

// Built-in functions
// Copy of the prototypes, but finished
int shell_cd(char **args) {
    // TODO: finish implementation
    (void) args;

    return 1;
}

// Global functions
int shell_execute(char **args) {
    (void) args;
    (void) built_in_command_funcs;
    num_built_ins();

    return 1;
}

int shell_launch_program(char **args) {
    (void) args;

    return 0;
}

int run_shell_command(char **args) {
    (void) args;

    return 0;
}
