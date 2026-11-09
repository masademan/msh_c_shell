#ifndef SHELL_COMMAND_MOVE_H
#define SHELL_COMMAND_MOVE_H

#include <stdbool.h>
#include <stddef.h>

#include "dynamic_array_char.h"

/* Uses ANSI codes and printf to move the cursor one character space to the left */
void move_cursor_left(void);

/* Uses ANSI codes and printf to move the cursor one character space to the right */
void move_cursor_right(void);

/* Move the cursor to the left by shift_amount character spaces
   There aren't any checks to see if it's off the screen,
   so make sure you know what you're doing */
void move_cursor_left_by(size_t shift_amount);

/* Move the cursor to the right by shift_amount character spaces
   There aren't any checks to see if it's off the screen,
   so make sure you know what you're doing */
void move_cursor_right_by(size_t shift_amount);

/* Moves the cursor from where it starts to where the specified end is */
void move_cursor_to(size_t *curr_position, size_t end);

/* Returns the amount that the cursor has to move left to get to the beginning of the next "word" */
size_t amount_to_move_cursor_to_next_left_word(const CharArray *line, size_t position);

/* Returns the amount that the cursor has to move right to get to the end of the next "word" */
size_t amount_to_move_cursor_to_next_right_word(const CharArray *line, size_t position);

/* Moves the cursor to the next left "word"
   Just like when you press Ctrl + left arrow on most text editors
   Returns how many characters it moved
   (direction insensitive, i.e., doesn't matter if it went left or right) */
size_t move_cursor_to_next_left_word(const CharArray *line, size_t *position);

/* Moves the cursor to the next right "word"
   Just like when you press Ctrl + right arrow on most text editors
   Returns how many characters it moved
   (direction insensitive, i.e., doesn't matter if it went left or right) */
size_t move_cursor_to_next_right_word(const CharArray *line, size_t *position);

#endif
