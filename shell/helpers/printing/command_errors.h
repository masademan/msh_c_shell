#ifndef COMMAND_ERRORS_H
#define COMMAND_ERRORS_H

/* Takes in the executable binary that was attempted to be run in a fork with execvp along with the error
   number Prints out the corresponding error message given the error number */
void fprintf_errno_message_fork(const char *executable_run, int err_num);

/* Takes in the directory that chdir tried to cd into and the error number
   Prints out the corresponding error message given the error number */
void fprintf_errno_message_chdir(const char *directory_cd_attempted, int err_num);

#endif
