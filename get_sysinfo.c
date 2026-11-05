#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "get_dir.h"

size_t size_increase_func(size_t size) {
    return size * 2;
}

char *get_hostname(void) {
    return "test";
}

char *get_dir(void) {
    size_t size = 64;
    char *cwd = malloc(size * sizeof(char));

    if (cwd == NULL) {
        perror("Initial CWD alloc failed");
        exit(EXIT_FAILURE);
    }

    while (getcwd(cwd, size) == NULL) {
        if (errno == ERANGE) {
            size = size_increase_func(size);
            if (size < 1) { size = 1; }
            free(cwd);
            cwd = malloc(size * sizeof(char));
            if (cwd == NULL) {
                perror("Expanding CWD alloc failed");
                exit(EXIT_FAILURE);
            }
        } else if (errno == EACCES) {
            perror("getcwd doesn't have permission to read this directory");
            free(cwd);
            exit(EXIT_FAILURE);
        } else {
            perror("getcwd error");
            free(cwd);
            exit(EXIT_FAILURE);
        }
    }

    return cwd;
}
