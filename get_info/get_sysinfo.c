#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "environ_var.h"
#include "get_sysinfo.h"
#include "shell/helpers/helpers.h"

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

bool is_in_home(const char *cwd, const char *home_dir) {
    if (cwd == NULL || home_dir == NULL) {
        return false;
    }
    if (cwd[0] == '\0' || home_dir[0] == '\0') {
        return false;
    }

    for (size_t i = 0; i < strlen(home_dir); i++) {
        if (cwd[i] != home_dir[i]) {
            return false;
        }
    }

    return true;
}

void shorten_cwd(char **cwd, const char *home_dir, const char *shortened_home) {
    if (cwd == NULL || *cwd == NULL || home_dir == NULL || shortened_home == NULL) {
        return;
    }
    if (home_dir[0] == '\0' || shortened_home[0] == '\0') {
        return;
    }
    if (!is_in_home(*cwd, home_dir)) {
        return;
    }
    size_t cwd_len = strlen(*cwd);
    size_t home_dir_len = strlen(home_dir);
    if (cwd_len < home_dir_len) {
        return;
    }

    size_t shortened_home_len = strlen(shortened_home);
    size_t new_cwd_len = cwd_len + shortened_home_len - home_dir_len;
    char *new_cwd = malloc(sizeof(char) * (new_cwd_len + 1));  // +1 for the \0
    if (new_cwd == NULL) {
        return;
    }

    size_t position;
    for (position = 0; position < shortened_home_len; position++) {
        new_cwd[position] = shortened_home[position];
    }

    for (size_t i = home_dir_len; i < cwd_len; i++) {
        new_cwd[position] = (*cwd)[i];
        position++;
    }

    new_cwd[position] = '\0';

    free(*cwd);
    *cwd = new_cwd;
}

char *get_shortened_cwd(void) {
    char *cwd = get_cwd();

    shorten_cwd(&cwd, get_environ_var_val("HOME"), "~");

    return cwd;
}
