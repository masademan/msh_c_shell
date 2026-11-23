#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "built_in_commands.h"
#include "get_info/environ_var.h"
#include "get_info/get_sysinfo.h"
#include "help.h"
#include "macros/ansi_colors.h"
#include "macros/shell_globals.h"
#include "shell/helpers/printing/command_errors.h"

// AI generated segment (NULL terminated string array)
extern char **environ;

char *built_in_command_names[] = {
    "cd",
    "exit",
    "export",
    "help",
};

int (*built_in_command_funcs[])(const char **, size_t) = {
    &shell_cd,
    &shell_exit,
    &shell_export,
    &shell_help,
};

bool is_built_in(const char *command_name) {
    for (size_t i = 0; i < num_built_ins(); i++) {
        if (strcmp(built_in_command_names[i], command_name) == 0) {
            return true;
        }
    }

    return false;
}

size_t num_built_ins(void) { return sizeof(built_in_command_names) / sizeof(char *); }

int shell_cd(const char **args, size_t num_args) {
    if (num_args > 2) {
        fprintf(stderr, "-%s: cd: too many arguments\n", SHELL_NAME);

        return 1;
    }

    const char *dir_to_cd_into;
    if (num_args == 1 || strcmp(args[1], "~") == 0) {
        // CD into root directory
        dir_to_cd_into = get_environ_var_val("HOME");
    } else if (strcmp(args[1], "-") == 0) {
        // CD into the previous directory
        dir_to_cd_into = get_environ_var_val("OLDPWD");

        if (dir_to_cd_into[0] == '\0') {
            fprintf(stderr, "-%s: cd: OLDPWD not set\n", SHELL_NAME);
            return 1;
        }

        printf("%s\n", dir_to_cd_into);
    } else {
        // CD into other directory
        dir_to_cd_into = args[1];
    }

    char *cwd = get_cwd();
    set_environ_var_val("OLDPWD", cwd, true);
    free(cwd);

    if (chdir(dir_to_cd_into) != 0) {
        int err_num = errno;
        fprintf_errno_message_chdir(dir_to_cd_into, err_num);

        return 1;
    }

    return 0;
}

int shell_exit(const char **args, size_t num_args) {
    (void) args;
    (void) num_args;

    printf("exit\n");

    return SHELL_EXIT_CODE;
}

// TODO: FINISH
int shell_export(const char **args, size_t num_args) {
    if (num_args > 2) {
        fprintf(stderr, "-%s: export: too many arguments\n", SHELL_NAME);

        return 1;
    }
    if (num_args == 1) {
        for (size_t i = 0; environ[i] != NULL; i++) {
            printf("declare -x %s\n", environ[i]);
        }
        return 0;
    }
    (void) args;

    // now do parsing and set it

    return 0;
}

int shell_help(const char **args, size_t num_args) {
    if (num_args == 1) {
        // Just help
        printf("Masa's SHell (" RESET BOLD "msh" RESET "), %s\n", SHELL_VERSION);
        printf("These shell commands are defined internally.  Type 'help' to see this list.\n");
        printf("Type 'help [name]' to find out more about the builtin command '[name]'.\n");
        printf("Use 'man -k' or 'info' to find out more about commands not in this list.\n");
        printf("Note:\n");
        printf(
            "This is a shell made for a C class as a final project. Many features found in other shells "
            "will be missing\n");
        printf(
            "But more features should come eventually, since this is an enjoyable project I'm learning "
            "from!\n\n");

        printf("Builtins:\n");
        for (size_t i = 0; i < num_built_ins(); i++) {
            printf(" - %s\n", built_in_command_names[i]);
        }
    } else {
        // More things
        for (size_t i = 1; i < num_args; i++) {
            if (is_built_in(args[i])) {
                printf_help_given_command(args[i]);
            } else {
                fprintf(stderr,
                        "-%s: help: command '%s' is not builtin, so it doesn't have a help page\n",
                        SHELL_NAME, args[i]);
            }
        }
    }

    return 0;
}
