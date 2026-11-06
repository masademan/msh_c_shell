#include "get_sysinfo.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t size_increase_func(size_t size) { return size * 2; }

char *get_username(void) {
    size_t size = 4;
    char *username = malloc(size * sizeof(char));

    if (username == NULL) {
        perror("Initial USERNAME alloc failed");
        exit(EXIT_FAILURE);
    }

    while (getlogin_r(username, size) != 0) {
        if (errno == ERANGE) {
            size = size_increase_func(size);
            if (size < 1) {
                size = 1;
            }

            free(username);

            username = malloc(size * sizeof(char));
            if (username == NULL) {
                perror("Expanding USERNAME alloc failed");
                exit(EXIT_FAILURE);
            }
        } else if (errno == ENXIO) {
            strcpy(username, "");
            break;
        } else {
            perror("getlogin_r error");
            free(username);
            exit(EXIT_FAILURE);
        }
    }

    return username;
}

char *get_hostname(void) {
    size_t size = 8;
    char *hostname = malloc(size * sizeof(char));

    if (hostname == NULL) {
        perror("Initial HOSTNAME alloc failed");
        exit(EXIT_FAILURE);
    }

    while (gethostname(hostname, size) == -1) {
        if (errno == ENAMETOOLONG) {
            size = size_increase_func(size);
            if (size < 1) {
                size = 1;
            }

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
    size_t size = 8;
    char *cwd = malloc(size * sizeof(char));

    if (cwd == NULL) {
        perror("Initial CWD alloc failed");
        exit(EXIT_FAILURE);
    }

    while (getcwd(cwd, size) == NULL) {
        if (errno == ERANGE) {
            size = size_increase_func(size);
            if (size < 1) {
                size = 1;
            }

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
