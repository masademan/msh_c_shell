#include <stdio.h>
#include <stdlib.h>

#include "get_dir.h"
#include "shell_loop.h"

int main(int arg_c, char **arg_v)
{
    // arg_c = argument count
    // arg_v = argument
    (void) arg_c;
    (void) arg_v;

    // Load configs (if there are any)

    // Run shell loop
    shell_loop();

    // Do any clean up when exiting

    return EXIT_SUCCESS;
}
