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
    //  config ideas:
    //      allow multi line commands (to save memory)
    //      allow print colors (idk)
    //      show username
    //      show machine
    //      max dir length (int, and is how many directories up to show, replace rest with ...,
    //          0 means show all)
    //      allow for arrow keys to reselect commands (also to save memory)
    //      rename project folder to "c shell - final project"
    //      maybe in the future do conditionals and loops in the shell (currently unused)

    // Run shell loop
    shell_loop();

    // Do any clean up when exiting

    return EXIT_SUCCESS;
}
