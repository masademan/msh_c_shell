#include <stdio.h>
#include <stdlib.h>

#include "shell_loop.h"
#include "shell_read_line.h"
#include "shell_parse_line.h"
#include "shell_execute.h"

void shell_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf(">> ");

        line = shell_read_line();
        args = shell_parse_line(line);
        status = shell_execute(args);

        free(line);
        free(args);
    } while (status);
}
