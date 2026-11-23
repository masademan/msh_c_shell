#ifndef SHELL_HISTORY_H
#define SHELL_HISTORY_H

#include <stdbool.h>

#include "data_type/dynamic_arrays/char_array.h"

/* Adds the inputted CharArray into the array of the commands run */
void add_command(const CharArray *command);

/* Removes the latest command run in the command history */
void pop_command(void);

/* Increments the command idx to the previous command
   This is to then get the command at the idx */
void set_command_idx_to_prev(void);

/* Decrements the command idx to the next command
   This is to then get the command at the idx */
void set_command_idx_to_next(void);

/* Set the command idx to the inputted index
   Returns true if successful
   Returns false if the idx is out of range */
bool set_command_idx(size_t idx);

/* Sets the inputted line to the command at the current idx
   Returns true if successful
   Returns false if line is NULL or any CharArray operation fails */
bool get_command(CharArray *line);

/* Run this before the program ends to free the array that is the command history */
void clean_up_command_history(void);

/* Run this at the beginning of the program to setup the command history array */
void setup_command_history(void);

#endif
