#ifndef SHELL_COMMAND_CHECK_H
#define SHELL_COMMAND_CHECK_H

#include <stdbool.h>

/* Takes in a character in int form (to allow EOF) and reads it
   Then it edits internal static variables, like whether or not there are open quotes,
   lines breaks, incomplete instructions, etc. */
void check_next_char(int chr);

/* Resets all the static internal variables */
void line_check_reset(void);

/* All the following functions use internal static variables to check their respective
   parts of the inputted command */

/* Returns true if all the checks for a complete command pass
   Else, returns false */
bool is_complete_command(void);

/* Returns true if there's an open set of quotes (and works with double and single quote mixing)
   Else, returns false */
bool has_open_quotes(void);

/* Returns true if there's an open set of parentheses
   Else, returns false */
bool has_open_parens(void);

/* Returns true if there's an open set of curly braces
   Else, returns false */
bool has_open_braces(void);

/* Returns true if there's an incomplete conditional (if, then, elif, else, fi)
   Else, returns false */
bool has_open_conditional(void);

/* Returns true if there's an incomplete loop (for, while, until)
   Else, returns false */
bool has_open_loop(void);

/* Returns true if there's a pipe symbol that hasn't been used (|)
   Else, returns false */
bool has_unfinished_pipe(void);

/* Returns true if there's a logical operator that hasn't been used (||, &&)
   Else, returns false */
bool has_unfinished_logical_operator(void);

/* Returns true if there's a backslash without a newline or space after it (\)
   Else, returns false */
bool has_unfinished_backslash(void);

#endif
