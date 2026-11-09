#ifndef SHELL_COMMAND_EDIT_H
#define SHELL_COMMAND_EDIT_H

#include <stddef.h>

#include "dynamic_array_char.h"

/* Used to show the changes after a backspace, delete, or mid-line character insert
   Prints all the chars from position until the end of the array
   Then sends the cursor back to where the position was
   (to keep the cursor in place after using the left + right arrow keys) */
void render_typed_command_after_removing_char(const CharArray *line, size_t position,
                                              size_t amount_removed);

/* Deletes any number of characters at a position and
   shifts the rest of the characters left by that amount */
void delete_characters_at_position_in_line(CharArray *line, size_t position, size_t amount_to_remove);

/* Prints the changes to the line in the terminal given the new character and where the cursor is in
   relation to the length of the command */
void show_line_changes(const CharArray *line, size_t *position, char c);

#endif
