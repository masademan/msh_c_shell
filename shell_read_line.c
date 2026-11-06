#include "shell_read_line.h"

#include <stdlib.h>
#include <string.h>

char *shell_read_line(void) {
    size_t size = 10;
    char *line = malloc(size * sizeof(char));

    strcpy(line, "test line");

    return line;
}
