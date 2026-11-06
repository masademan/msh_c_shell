#include <stdio.h>

#include "shell_helpers.h"

size_t size_increase_func(size_t size) { return size * 2; }

void clear_terminal() { printf("\033[1;1H\033[2J"); }
