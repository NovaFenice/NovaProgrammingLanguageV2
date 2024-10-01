#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/ast.h"
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

void print_ast(ast_T* node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) printf("  ");
    
    switch (node->type) {
        case AST_VARIABLE_DECLARATION:
            printf("Variable Declaration: %s\n", node->token->value);
            break;
        case AST_ASSIGNMENT:
            printf("Assignment: %s\n", node->token->value);
            break;
        case AST_BINARY_OP:
            printf("Binary Operation: %s\n", node->token->value);
            break;
        case AST_LITERAL:
            printf("Literal: %s\n", node->value);
            break;
        case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->value);
            break;
        default:
            printf("Unknown AST Node\n");
            break;
    }

    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: No input file provided\n");
        return 1;
    }

    if (!is_nova_format(argv[1])) {
        printf("Error: The file should be in .nova format\n");
        return 1;
    }

    char* file_contents = read_file(argv[1]);
    if (!file_contents) {
        return 1;
    }

    lexer_T* lexer = init_lexer(file_contents);
    parser_T* parser = init_parser(lexer);

    printf("Abstract Syntax Tree:\n");
    while (parser->current_token->type != TOKEN_E0F) {
        ast_T* root = parser_parse(parser);
        if (root) {
            //print_ast(root, 0);
        } else {
            //printf("Error: Could not generate the AST\n");
        }
    }

    free(file_contents);
    return 0;
}