#ifndef SHELL_HELPERS_H
#define SHELL_HELPERS_H

#include <stddef.h>

/* Takes in a size_t size and increases it. Could be doubled or increased by a
   constant amount. This function will be run in a function when a string needs
   to be bigger to fit more characters in the allocated memory. Then the output
   of this function will be the new amount of memory allocated
   If the function returns a number smaller than the inputted size, then the
   code will act in an undefined way, so make sure the function can only
   increases the value (not counting overflows) */
size_t size_increase_func(size_t size);

/* Clears the terminal where the shell is being run
   Good to clear when the program starts so the shell looks more seamless
   Also useful for the shell command "clear" */
void clear_terminal(void);

/* Takes in the executable that was attempted to be run as well as the error number from errno and
   fprintf's the appropiate error message */
void fprintf_errno_message_fork(const char *executable_run, int err_num);

/* Takes in the error number from errno after running chdir and outputs the appropiate
   error message */
void fprintf_errno_message_chdir(int err_num);

#endif
