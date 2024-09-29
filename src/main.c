#include <stdio.h>
#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filename){
    FILE* file = fopen(filename, "r");
    if(!file){
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* buffer = calloc(file_size + 1, sizeof(char));
    if (!buffer){
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}

int is_nova_format(const char* filename) {
    const char* extension = strrchr(filename, '.');
    if (extension && strcmp(extension, ".nova") == 0) {
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Error: No input file provided\n");
        return 1;
    }

    if (!is_nova_format(argv[1])){
        printf("Error: The file should be nova format\n");
        return 1;
    }

    char* file_contents = read_file(argv[1]);
    if(!file_contents){
        return 1;
    }

    lexer_T* lexer = init_lexer(file_contents);

    token_T* token = (void*)0;

    while((token = lexer_get_next_token(lexer)) != (void*)0){
        printf("TOKEN(%d, %s)\n", token->type, token->value);
    }

    free(file_contents);

    return 0;
}