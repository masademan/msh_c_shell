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
    char *cwd;
    char *line;
    char **args;
    int status;

    do {
        // Printing the dir
        // HI_GREEN ; HI_BLUE
        cwd = get_dir();
        printf(HI_BLUE "%s" RESET, cwd);

        // Printing the starting point of the shell commands
        printf("%s \n", shell_start);

        line = shell_read_line();
        args = shell_parse_line(line);
        status = shell_execute(args);
    } while (status);

    free(cwd);
    free(line);
    free(args);
}
