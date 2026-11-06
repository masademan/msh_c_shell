#include <stdio.h>
#include <stdlib.h>

#include "ansi_colors.h"
#include "get_sysinfo.h"
#include "shell_execute.h"
#include "shell_helpers.h"
#include "shell_loop.h"
#include "shell_parse_line.h"
#include "shell_read_line.h"

const char *shell_start = ">>";

void shell_loop(void) {
    char *username;
    char *hostname;
    char *cwd;
    char *line;
    char **args;
    int status;

    clear_terminal();

    // TODO: Use configs that are set up in main.c to turn on/off the user and machine name

    do {
        // Printing the dir
        // HI_GREEN ; HI_BLUE
        username = get_username();
        hostname = get_hostname();
        cwd = get_cwd();

        if (username[0] != '\0') {  // Has username
            printf(HI_GREEN "%s@" RESET, username);
        }
        printf(HI_GREEN "%s" RESET ":" HI_BLUE "%s" RESET, hostname, cwd);

        // Printing the starting point of the shell commands
        printf("%s ", shell_start);

        line = shell_read_line();
        printf("\n\"\"\"\n%s\n\"\"\"\n", line);
        args = shell_parse_line(line);
        status = shell_execute(args);
    } while (status);

    free(username);
    free(hostname);
    free(cwd);
    free(line);
    free(args);
}
