#include <stdio.h>
#include <stdlib.h>

#include "get_dir.h"

#include "ansi_colors.h"

#include "shell_loop.h"
#include "shell_read_line.h"
#include "shell_parse_line.h"
#include "shell_execute.h"

const char *shell_start = ">>";

void shell_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf(HI_GREEN "%s" RESET, get_dir());        // Printing the dir
        printf("%s \n", shell_start);  // Printing the starting point of the shell commands

        line = shell_read_line();
        args = shell_parse_line(line);
        status = shell_execute(args);

        free(line);
        free(args);
    } while (status);
}
