#include "parser.h"
#include <stdio.h>

i32 main(i32 argc, char **argv) {
    if (argc < 2) {
        printf("\033[31mNo input file.\n\033[0m");
        return NO_INPUT;
    }

    return 0;
}