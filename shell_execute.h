#ifndef SHELL_EXECUTE_H
#define SHELL_EXECUTE_H

#include "dynamic_array_string.h"

/* Takes in a dynamic array of strings (pointer to a pointer to a char) and
   starts a process based on the args given
   An int is then returned, which is the status code after finishing */
int shell_execute(StringArray *args);

/* Takes in a dynamic array of strings (pointer to a pointer to a char) and
   starts launching a program from the PATH
   An int is then returned, which is the status code after finishing */
int shell_launch_program(StringArray *args);

/* Takes in a dynamic array of strings (pointer to a pointer to a char) and
   starts running a command built directly into the shell
   An int is then returned, which is the status code after finishing */
int run_shell_command(StringArray *args);

#endif
