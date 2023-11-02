#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

i32 main(i32 argc, char **argv) {
    if (argc < 2) {
        printf("\033[31mNo input file.\n\033[0m");
        exit(NO_INPUT);
    }

    FILE *input_file = fopen(argv[1], "rb");
    
    if(!input_file) {
        printf("\033[31mFile not found.\n\033[0m");
        exit(INPUT_NOT_FOUND);
    }

    fseek(input_file, 0, SEEK_END);
    u64 file_size = ftell(input_file) * 2;
    rewind(input_file);
    char *content = malloc(file_size + 1);
    memset(content, 0, file_size + 1);
    fread(content, 1, file_size, input_file);
    content[file_size] = '\0';
    fclose(input_file);
    
    parse(content);

    return 0;
}