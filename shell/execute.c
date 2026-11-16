#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "execute.h"
#include "helpers/arguments/split.h"
#include "helpers/commands/check.h"
#include "helpers/helpers.h"
#include "macros/shell_globals.h"

// Built-in function prototypes
// Example:
//   int shell_cd(const char **args, size_t num_args);
static int shell_cd(const char **args, size_t num_args);
static int shell_exit(const char **args, size_t num_args);
static int shell_help(const char **args, size_t num_args);

// Built-in setup
static char *built_in_command_names[] = {
    "cd",
    "exit",
    "help",
};

static int (*built_in_command_funcs[])(const char **, size_t) = {
    &shell_cd,
    &shell_exit,
    &shell_help,
};

// Make a list for the built-in commands in the same order as the built_in_commands array

static size_t num_built_ins(void) { return sizeof(built_in_command_names) / sizeof(char *); }

// Built-in functions
// Copy of the prototypes, but finished
static int shell_cd(const char **args, size_t num_args) {
    if (num_args > 2) {
        fprintf(stderr, "-%s: cd: too many arguments\n", SHELL_NAME);

        return 1;
    }
    // TODO: finish implementation
    (void) args;
    (void) num_args;

    return 0;
}

static int shell_exit(const char **args, size_t num_args) {
    (void) args;
    (void) num_args;

    return SHELL_EXIT_CODE;
}

static int shell_help(const char **args, size_t num_args) {
    // TODO: finish implementation
    (void) args;
    (void) num_args;

    return 0;
}

// Static functions
/* This is to find where the next token is
   Usually the offset is 1 (very next token), but if there are
   parentheses, the offset is higher since those part are handled with forks,
   which have to merge back with the parent process */
static size_t offset_to_next_token(const StringArray *tokens, size_t curr_idx) {
    (void) tokens;
    (void) curr_idx;

    line_check_reset();

    return 0;
}

// Global functions
// make it so that ctrl + c kills the child process, end the execution in the parent, and return 1
//  should bring user back to shell
int shell_execute(const StringArray *tokens) {
    if (da_count_string(tokens) == 0) {
        return 0;
    }

    offset_to_next_token(tokens, 0);

    // printf("Tokens:\n");
    // String *s_token;
    // for (size_t token_i = 0; token_i < da_count_string(tokens); token_i++) {
    //     if (!da_get_string(tokens, token_i, &s_token)) {
    //         fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
    //         return 1;
    //     }
    //
    //     printf(" - token %zu: ⟨%s⟩\n", token_i + 1, ss_cstr(s_token));
    // }

    if (da_count_string(tokens) > 0) {
        String *token;

        if (!da_get_string(tokens, 0, &token)) {
            fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
            return 0;
        }

        return shell_execute_command(token);
    }

    return 0;
}

int shell_execute_command(const String *token) {
    StringArray *string_args = shell_token_to_arguments(token);
    const char **args = string_arg_array_to_execvp_arg_array(string_args);

    if (string_args == NULL) {
        free(args);

        return 0;
    }
    if (args == NULL) {
        da_free_string(string_args);

        return 0;
    }
    if (da_count_string(string_args) == 0) {
        free(args);
        da_free_string(string_args);

        return 0;
    }

    int status = 0;
    bool built_in_done = false;

    // printf("Token args:\n");
    // for (size_t i = 0; args[i] != NULL; i++) {
    //     printf(" - arg %zu: ⟨%s⟩\n", i + 1, args[i]);
    // }

    for (size_t i = 0; i < num_built_ins(); i++) {
        if (strcmp(args[0], built_in_command_names[i]) == 0) {
            status = (*built_in_command_funcs[i])(args, da_count_string(string_args));
            built_in_done = true;
            break;
        }
    }

    if (!built_in_done) {
        status = shell_launch_program(args);
    }

    free(args);
    da_free_string(string_args);

    return status;
}

int shell_launch_program(const char **args) {
    pid_t pid, wpid;
    int status = 0;

    pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], (char **) args);

        int err_num = errno;
        fprintf_errno_message_fork(args[0], err_num);

        _exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        fprintf(stderr, "-%s: Error forking\n", SHELL_NAME);

        return 1;
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);

            if (wpid == -1) {
                if (errno == EINTR) {
                    continue;
                }

                perror("waitpid");
                return 1;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            return 1;
        }

        return 1;
    }
}
