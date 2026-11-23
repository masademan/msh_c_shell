#ifndef BUILT_IN_COMMANDS_H
#define BUILT_IN_COMMANDS_H

#include <stdbool.h>
#include <stddef.h>

extern char *built_in_command_names[];
extern int (*built_in_command_funcs[])(const char **, size_t);

/* Returns true if the string given is equal to the name of any of the built in commands */
bool is_built_in(const char *command_name);

/* Returns the number of built in commands */
size_t num_built_ins(void);

int shell_cd(const char **args, size_t num_args);
int shell_exit(const char **args, size_t num_args);
int shell_export(const char **args, size_t num_args);
int shell_help(const char **args, size_t num_args);

#endif
