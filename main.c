#include <stdlib.h>

#include "shell_loop.h"

int main(int arg_c, char **arg_v) {
    // arg_c = argument count
    // arg_v = argument
    (void) arg_c;
    (void) arg_v;

    // Load configs (if there are any)
    // TODO: Find a way to load configs using arg_c and arg_v, then input that to shell_loop
    //          somehow

    // Run shell loop
    shell_loop();

    // Do any clean up when exiting

    return EXIT_SUCCESS;
}
