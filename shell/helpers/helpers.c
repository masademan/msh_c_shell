#include <stdio.h>

#include "helpers.h"

size_t size_increase_func(size_t size) { return size * 2; }

// AI generated ANSI escape codes
void clear_terminal(void) { printf("\033[1;1H\033[2J\033[3J"); }
