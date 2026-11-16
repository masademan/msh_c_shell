#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>  // IWYU pragma: keep
#include <stdio.h>
#include <string.h>

#include "helpers.h"
#include "macros/shell_globals.h"

size_t size_increase_func(size_t size) { return size * 2; }

// AI generated ANSI escape codes
void clear_terminal(void) { printf("\033[1;1H\033[2J\033[3J"); }

void fprintf_errno_message_fork(const char *executable_run, int err_num) {
    const char *msg;

    switch (err_num) {
        case ENOENT:
            // File not found
            msg = strchr(executable_run, '/') ? "No such file or directory" : "command not found";
            break;
        case EACCES:
            // Permission denied
            msg = "Permission denied";
            break;
        case ENOEXEC:
            // Header format not recognized
            msg = "Executable format error";
            break;
        case E2BIG:
            // Number of args exceeds system limit
            msg = "Number of arguments exceeds system limits";
            break;
        case ENOTDIR:
            // Part of the path is a file, not a directory
            msg = "A component of the path is not a directory";
            break;
        case EISDIR:
            // A directory was attempted to be run, not an executable file
            msg = "Is a directory";
            break;
        case ENOMEM:
            // Not enough memory to load the new process
            msg = "Not enough memory to start process";
            break;
        case EPERM:
            // Similar to EACESS, but for WSL because Windows
            msg = "Operation not permitted";
            break;
        case ELOOP:
            // Too many symbolic links while resolving path
            msg = "Too many symbolic links encountered while resolving path";
            break;
        case ENAMETOOLONG:
            // Filename/path name exceeds system limits
            msg = "Filename or path exceeds system length limits";
            break;
        case ETXTBSY:
            msg = "Executable file is busy or recently open for writing";
            break;
        default:
            msg = strerror(err_num);
            break;
    }

    fprintf(stderr, "-%s: %s: %s\n", SHELL_NAME, executable_run, msg);
}

void fprintf_errno_message_chdir(const char *directory_cd_attempted, int err_num) {
    const char *msg;

    switch (err_num) {
        case ENOENT:
            msg = "No such file or directory";
            break;
        case ENOTDIR:
            msg = "Not a directory";
            break;
        case EACCES:
            msg = "Permission denied";
            break;
        case ELOOP:
            msg = "Too many symbolic links encountered while resolving path";
            break;
        case ENAMETOOLONG:
            msg = "Path exceeds system length limits";
            break;
        default:
            msg = strerror(err_num);
            break;
    }

    fprintf(stderr, "-%s: cd: %s: %s\n", SHELL_NAME, directory_cd_attempted, msg);
    return;
}
