#ifndef SHELL_EXECUTE_H
#define SHELL_EXECUTE_H

#include "data_type/dynamic_arrays/string_array.h"

/* Takes in a dynamic array of strings and
   starts a process based on the args given
   An int is then returned, which is the status code after finishing */
int shell_execute(const StringArray *tokens);

/* Takes in a single token (command + its args) and splits
   it into an array of strings
   The first string is the command, and the following strings (if there are any)
   are the arguments for that command */
StringArray *shell_token_to_arguments(const String *token);

/* Takes in a dynamic array of strings and
   splits into the shell_launch_program or
   run_shell_command depending on the first argument (command name)
   This function also passes the arguments to either function and
   returnst the same exit code as whichever runs */
int shell_execute_command(const StringArray *args);

/* Takes in a dynamic array of strings and
   starts launching a program from the PATH
   An int is then returned, which is the status code after finishing */
int shell_launch_program(const StringArray *args);

/* Takes in a dynamic array of strings and
   starts running a command built directly into the shell
   An int is then returned, which is the status code after finishing */
int run_shell_command(const StringArray *args);

#endif
