// #include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "data_type/dynamic_arrays/char_array.h"
#include "data_type/dynamic_arrays/char_array_and_string_conversion.h"
#include "data_type/dynamic_arrays/string_array.h"
#include "data_type/safe_string/safe_string.h"
#include "execute.h"
#include "helpers/arguments/clean.h"
#include "helpers/commands/check.h"
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

    return 1;
}

static int shell_exit(const char **args, size_t num_args) {
    (void) args;
    (void) num_args;

    return 0;
}

static int shell_help(const char **args, size_t num_args) {
    // TODO: finish implementation
    (void) args;
    (void) num_args;

    return 1;
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

/* Takes in a single token (command + its args) and splits
   it into an array of strings
   The first string is the command, and the following strings (if there are any)
   are the arguments for that command */
static StringArray *shell_token_to_arguments(const String *token) {
    StringArray *args = da_new_string();
    CharArray *temp_arg_arr = da_new_char();
    String *temp_arg_string;
    char c;

    line_check_reset();

    for (size_t i = 0; i < ss_length(token); i++) {
        if (!ss_at(token, i, &c)) {
            da_free_char(temp_arg_arr);
            da_free_string(args);

            fprintf(stderr, "-%s: ss_at error\n", SHELL_NAME);
            return NULL;
        }

        check_next_char(c);

        if (!has_open_quotes() && c == ' ') {
            temp_arg_string = da_join_to_string(temp_arg_arr);

            clean_argument(&temp_arg_string);

            if (!da_add_string(args, temp_arg_string)) {
                ss_free(temp_arg_string);
                da_free_char(temp_arg_arr);
                da_free_string(args);

                fprintf(stderr, "-%s: da_add error\n", SHELL_NAME);
                return NULL;
            }

            ss_free(temp_arg_string);
            da_clear_char(temp_arg_arr);
        } else {
            if (!da_add_char(temp_arg_arr, c)) {
                da_free_char(temp_arg_arr);
                da_free_string(args);

                fprintf(stderr, "-%s: da_add error\n", SHELL_NAME);
                return NULL;
            }
        }
    }

    if (da_count_char(temp_arg_arr) > 0) {
        temp_arg_string = da_join_to_string(temp_arg_arr);

        clean_argument(&temp_arg_string);

        if (!da_add_string(args, temp_arg_string)) {
            ss_free(temp_arg_string);
            da_free_char(temp_arg_arr);
            da_free_string(args);

            fprintf(stderr, "-%s: da_add error\n", SHELL_NAME);
            return NULL;
        }

        ss_free(temp_arg_string);
        da_clear_char(temp_arg_arr);
    }

    da_free_char(temp_arg_arr);

    return args;
}

/* Takes an array of Strings and outputs an array of strings (array of chars) that's
   NULL terminated
   This is to make freeing much easier (just free the pointer returned, then free the StringArray later)
   The caller owns the returned pointer and has to call free() on it to free it */
static const char **string_arg_array_to_execvp_arg_array(const StringArray *string_args) {
    if (string_args == NULL) {
        return NULL;
    }

    size_t num_args = da_count_string(string_args);
    const char **char_args = malloc(sizeof(char *) * (num_args + 1));
    if (char_args == NULL) {
        fprintf(stderr, "-%s: couldn't allocate mamoery for command arguments\n", SHELL_NAME);
        return NULL;
    }

    String *string_arg;
    for (size_t i = 0; i < num_args; i++) {
        if (!da_get_string(string_args, i, &string_arg)) {
            free(char_args);

            fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
            return NULL;
        }

        char_args[i] = ss_cstr(string_arg);
    }

    char_args[num_args] = NULL;

    return char_args;
}

// Global functions
// make it so that ctrl + c kills the child process, end the execution in the parent, and return 1
//  should bring user back to shell
int shell_execute(const StringArray *tokens) {
    if (da_count_string(tokens) == 0) {
        return 1;
    }

    offset_to_next_token(tokens, 0);

    printf("Tokens:\n");
    String *s_token;
    for (size_t token_i = 0; token_i < da_count_string(tokens); token_i++) {
        if (!da_get_string(tokens, token_i, &s_token)) {
            fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
            return 1;
        }

        printf(" - token %zu: ⟨%s⟩\n", token_i + 1, ss_cstr(s_token));
    }

    if (da_count_string(tokens) > 0) {
        String *token;

        if (!da_get_string(tokens, 0, &token)) {
            fprintf(stderr, "-%s: da_get error\n", SHELL_NAME);
            return 1;
        }

        return shell_execute_command(token);
    }

    return 1;
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

    int status = 1;
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
    (void) args;

    return 1;
}
