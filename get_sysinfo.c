#define _POSIX_C_SOURCE 200112L // AI assisted code to make gethostname work

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "get_sysinfo.h"

size_t size_increase_func(size_t size) {
    return size * 2;
}

#include <string.h>
char *get_username(void) {
    size_t size = 64;
    char *username = malloc(size * sizeof(char));

    if (username == NULL) {
        perror("Initial USERNAME alloc failed");
        exit(EXIT_FAILURE);
    }

    strcpy(username, "bob");

    return username;
}

char *get_hostname(void) {
    size_t size = 64;
    char *hostname = malloc(size * sizeof(char));

    if (hostname == NULL) {
        perror("Initial HOSTNAME alloc failed");
        exit(EXIT_FAILURE);
    }

    while (gethostname(hostname, size) == -1) {
        if (errno == ENAMETOOLONG) {
            size = size_increase_func(size);
            if (size < 1) { size = 1; }

            free(hostname);

            hostname = malloc(size * sizeof(char));
            if (hostname == NULL) {
                perror("Expanding HOSTNAME alloc failed");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("gethostname error");
            free(hostname);
            exit(EXIT_FAILURE);
        }
    }

    return hostname;
}

char *get_cwd(void) {
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
