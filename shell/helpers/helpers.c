#include <stdio.h>

#include "helpers.h"

size_t size_increase_func(size_t size) { return size * 2; }

// AI generated ANSI escape codes
void clear_terminal(void) { printf("\033[1;1H\033[2J\033[3J"); }

bool char_is_num(char c) { return '0' <= c && c <= '9'; }

bool char_is_alpha_or_underscore(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_');
}
