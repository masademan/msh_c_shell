#include <stdio.h>
#include <stdlib.h>

#include "ansi_colors.h"
#include "dynamic_array_char.h"
#include "get_sysinfo.h"
#include "shell_execute.h"
#include "shell_helpers.h"
#include "shell_loop.h"
#include "shell_parse_command.h"
#include "shell_read_command.h"

static const char *shell_start = ">>";

static void free_vars(char **username, char **hostname, char **cwd, CharArray **command) {
    // Shell prompt
    free(*username);
    free(*hostname);
    free(*cwd);

    *username = NULL;
    *hostname = NULL;
    *cwd = NULL;


    // Command
    da_free_char(*command);

    *command = NULL;


    // Parsed command arguments
}

void shell_loop(void) {
    char *username;
    char *hostname;
    char *cwd;
    CharArray *command;
    char **args;
    int status;

    clear_terminal();

    // TODO: Use configs that are set up in main.c to turn on/off the user and machine name
    //       Allow for arrow keys to select previous commands and stuff

    do {
        // Printing the dir
        // HI_GREEN ; HI_BLUE
        username = get_username();
        hostname = get_hostname();
        cwd = get_cwd();

        /*
        if (username[0] != '\0') {  // Has username
            printf(HI_GREEN "%s@" RESET, username);
        }
        printf(HI_GREEN "%s" RESET ":" HI_BLUE "%s" RESET, hostname, cwd);
        */
        printf(HI_BLUE "testing directory" RESET);

        // Printing the starting point of the shell commands
        printf("%s ", shell_start);

        // Read command
        command = shell_read_command();

        if (command == NULL || da_count_char(command) == 0) {
            status = 1;

            // TODO: REMOVE LATER
            if (command != NULL) {
                free_vars(&username, &hostname, &cwd, &command);
                break;
            }

        } else {
            args = shell_parse_line(command);
            status = shell_execute(args);
        }

        // Pointer clean-up
        free_vars(&username, &hostname, &cwd, &command);
    } while (status);
}
