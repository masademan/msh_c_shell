#ifndef SHELL_PARSE_LINE_H
#define SHELL_PARSE_LINE_H

/* Takes in a string from the shell_read_line function and parses it into
   individual tokens This returns a dynamic array of string (pointer to a
   pointer to a char) which can be inputted into the shell_execute function
   The caller owns the pointer returned and has to call arg_free() on it to free it
 */
char **shell_parse_line(char *line);

/* Used to free the dynamic array of strings (pointer to a pointer to a char)
   If args is NULL, it does nothing */
void arg_free(char **args);

#endif
