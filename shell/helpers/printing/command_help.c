#include <stdio.h>
#include <string.h>

#include "command_help.h"

void printf_help_given_command(const char *command) {
    if (strcmp(command, "cd") == 0) {
        printf("cd: cd [dir]\n");
        printf("\tChange the shell working directory.\n\n");
        printf(
            "\tChange the current directory to DIR. If DIR is \"-\", it is converted to $OLDPWD.\n\n");
        printf("\t'..' is processed by removing the immediately previous pathname component\n");
        printf("\tback to a slash of the beginning of DIR.\n\n");
        printf("\tExit Status:\n");
        printf("\tReturns 0 if the directory is changed; non-zero otherwise.\n");
    } else if (strcmp(command, "exit") == 0) {
        printf("exit: exit\n");
        printf("\tExit the shell.\n\n");
        printf("\tExits the shell with a special exit status.\n");
    } else if (strcmp(command, "help") == 0) {
        printf("help: help [pattern ...]\n");
        printf("\tDisplay information about builtin commands.\n\n");
        printf("\tDisplays brief summaries of builtin commands. If PATTERN is\n");
        printf("\tspecified, gives detailed help on all commands matching PATTERN,\n");
        printf("\totherwise the list of help topics is printed.\n\n");
        printf("\tArguments:\n");
        printf("\t\t PATTERN\tPattern specifying a help topic\n\n");
        printf("\tExit Status:\n");
        printf("\tReturns success unless PATTERN is not found.\n");
    } else if (strcmp(command, "export") == 0) {
        printf("export: export [name=value]\n");
        printf("\tSet environment variables in the shell.\n\n");
        printf("\tExports the NAME to the list of environment variables and\n");
        printf("\tsets that variable to VALUE.\n\n");
        printf("\tIf no NAME or VALUE is supplied, all the environment\n");
        printf("\tvariables and their values will be outputted.\n\n");
        printf("\tExit Status:\n");
        printf("\tReturns success unless an NAME is invalid.\n");
    }
}
