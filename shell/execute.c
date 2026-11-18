#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "execute.h"
#include "get_info/environ_var.h"
#include "get_info/get_sysinfo.h"
#include "helpers/arguments/split.h"
#include "helpers/commands/check.h"
#include "helpers/helpers.h"
#include "macros/ansi_colors.h"
#include "macros/shell_globals.h"

#define NOTHING_MODE 0
#define AND_MODE 1
#define OR_MODE 2

// Built-in function prototypes
static int shell_cd(const char **args, size_t num_args);
static int shell_exit(const char **args, size_t num_args);
static int shell_export(const char **args, size_t num_args);
static int shell_help(const char **args, size_t num_args);

// Built-in setup
static char *builtin_command_names[] = {
    "cd",
    "exit",
    "export",
    "help",
};

static int (*builtin_command_funcs[])(const char **, size_t) = {
    &shell_cd,
    &shell_exit,
    &shell_export,
    &shell_help,
};

static size_t num_builtins(void) { return sizeof(builtin_command_names) / sizeof(char *); }

static bool is_builtin(const char *command_name) {
    for (size_t i = 0; i < num_builtins(); i++) {
        if (strcmp(builtin_command_names[i], command_name) == 0) {
            return true;
        }
    }

    return false;
}

// Built-in functions
// TODO: ADD ~ WHEN IT'S JUST cd
static int shell_cd(const char **args, size_t num_args) {
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

static int shell_exit(const char **args, size_t num_args) {
    (void) args;
    (void) num_args;

    printf("exit\n");

    return SHELL_EXIT_CODE;
}

// TODO: FINISH
static int shell_export(const char **args, size_t num_args) {
    (void) args;
    (void) num_args;

    return 0;
}

static int shell_help(const char **args, size_t num_args) {
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
        for (size_t i = 0; i < num_builtins(); i++) {
            printf(" - %s\n", builtin_command_names[i]);
        }
    } else {
        // More things
        for (size_t i = 1; i < num_args; i++) {
            if (is_builtin(args[i])) {
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

// Static functions
/* Returns true if the String token is equal to the string (char *) test_token
   Else, returns false */
static bool is_token(const String *token, const char *test_token) {
    return strcmp(ss_cstr(token), test_token) == 0;
}

/* This is to find where the next token is
   Usually the offset is 1 (very next token), but if there are
   parentheses, the offset is higher since those part are handled with forks,
   which have to merge back with the parent process
   Only parentheses make the token skip more than 1 */
static size_t offset_to_next_token(const StringArray *tokens, size_t curr_idx) {
    line_check_reset();
    set_do_error(false);

    String *token;
    size_t token_idx;
    for (token_idx = curr_idx; token_idx < da_count_string(tokens); token_idx++) {
        if (!da_get_string(tokens, token_idx, &token)) {
            fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
            return 0;
        }

        if (is_token(token, "(")) {
            check_next_char('(');
            check_next_char('m');
        } else if (is_token(token, ")")) {
            check_next_char(')');
        }

        if (!has_open_parens()) {
            break;
        }
    }

    return 1 + token_idx - curr_idx;
}

/* When shell_execute reaches a '(', it runs this function to fork the process
   and run a recursive call of shell_execute
   This returns the status code of the recursive call of shell_execute */
static int fork_for_paren(const StringArray *tokens, size_t token_idx) {
    pid_t pid, wpid;
    int status = 0;

    pid = fork();
    if (pid == 0) {
        // Child process
        _exit(shell_execute(tokens, token_idx));
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

// Global functions
// make it so that ctrl + c kills the child process, end the execution in the parent, and return 1
//  should bring user back to shell
int shell_execute(const StringArray *tokens, size_t token_idx) {
    if (da_count_string(tokens) == 0) {
        return 0;
    }

    // AI generated code (from read_command.c, just modified):
    struct sigaction sa_ignore;
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;
    sigaction(SIGINT, &sa_ignore, NULL);

    String *token;
    int command_status = -1;
    int token_mode = NOTHING_MODE;

    // TODO: REMOVE
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

    // return 0;

    while (token_idx < da_count_string(tokens)) {
        if (!da_get_string(tokens, token_idx, &token)) {
            fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
            return 1;
        }

        // Token checking
        //   &&, ||, |, (), {}, ;
        if (is_token(token, "&&")) {
            token_mode = AND_MODE;
        } else if (is_token(token, "||")) {
            token_mode = OR_MODE;
        } else if (is_token(token, "|")) {
            // TODO: finish fully
            token_mode = NOTHING_MODE;
        } else if (is_token(token, "(")) {
            if ((command_status == -1) ||  // A command hasn't been done yet
                (command_status == 0 &&
                 token_mode == AND_MODE) ||  // Previous command was successful and operator is &&
                (command_status != 0 &&
                 token_mode == OR_MODE) ||     // Previous command failed and operator is ||
                token_mode == NOTHING_MODE) {  // Continues if it's a nothing mode
                command_status = fork_for_paren(tokens, token_idx + 1);
            }
        } else if (is_token(token, ")")) {
            break;
        } else if (is_token(token, "{")) {
            token_mode = NOTHING_MODE;
            token_idx++;
            continue;
        } else if (is_token(token, "}")) {
            token_mode = NOTHING_MODE;
            token_idx++;
            continue;
        } else if (is_token(token, ";")) {
            token_mode = NOTHING_MODE;
        } else {
            // Execute command
            if ((command_status == -1) ||  // A command hasn't been done yet
                (command_status == 0 &&
                 token_mode == AND_MODE) ||  // Previous command was successful and operator is &&
                (command_status != 0 &&
                 token_mode == OR_MODE) ||     // Previous command failed and operator is ||
                token_mode == NOTHING_MODE) {  // Continues if it's a nothing mode
                // This is comes from a simplified if statement chain (very nice simplification)
                command_status = shell_execute_command(token);
            }
        }


        token_idx += offset_to_next_token(tokens, token_idx);
    }

    return command_status;
}

int shell_execute_command(const String *token) {
    StringArray *string_args = shell_token_to_arguments(token);
    const char **args = string_arg_array_to_execvp_arg_array(string_args);

    if (string_args == NULL) {
        free(args);

        return 1;
    }
    if (args == NULL) {
        da_free_string(string_args);

        return 1;
    }
    if (da_count_string(string_args) == 0) {
        free(args);
        da_free_string(string_args);

        return 1;
    }

    int status = 0;
    bool built_in_done = false;

    // TODO: REMOVE
    // printf("Token args:\n");
    // for (size_t i = 0; args[i] != NULL; i++) {
    //     printf(" - arg %zu: ⟨%s⟩\n", i + 1, args[i]);
    // }

    for (size_t i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_command_names[i]) == 0) {
            status = (*builtin_command_funcs[i])(args, da_count_string(string_args));
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
        // AI generated code (from read_command.c, just modified):
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);

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
