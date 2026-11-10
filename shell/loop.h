#ifndef SHELL_LOOP_H
#define SHELL_LOOP_H

/* The shell loop for msh
   Uses shell_parse_line.h, shell_read_line.h, and shell_execute_line.h in the loop
   It follows the loop: Read, Parse, Execute, Repeat
   Runs functions in the 3 header files mentioned
   Function ends when a specific status code has been reached */
void shell_loop(void);

#endif
