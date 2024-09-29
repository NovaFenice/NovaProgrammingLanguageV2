#include "include/lexer.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#define BUFFER_SIZE 64

lexer_T* init_lexer(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(lexer_T));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_T* lexer) {
    lexer->i++;
    lexer->c = lexer->contents[lexer->i];
}

void lexer_skip_whitespace(lexer_T* lexer) {
    while (lexer->c == ' ' || lexer->c == '\n' || lexer->c == '\t') {
        lexer_advance(lexer);
    }
}

token_T* lexer_collect_id(lexer_T* lexer) {
    char buffer[BUFFER_SIZE];
    int buffer_index = 0;

    while (isalnum(lexer->c) && buffer_index < BUFFER_SIZE - 1) {
        buffer[buffer_index++] = lexer->c;
        lexer_advance(lexer);
    }

    buffer[buffer_index] = '\0';

    return init_token(TOKEN_ID, strdup(buffer));
}

token_T* lexer_get_next_token(lexer_T* lexer) {
    while (lexer->c != '\0') {
        if (lexer->c == ' ' || lexer->c == '\n' || lexer->c == '\t') {
            lexer_skip_whitespace(lexer);
        }

        if (isalnum(lexer->c)) {
            return lexer_collect_id(lexer);
        }

        switch (lexer->c) {
            case '=': return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, strdup("=")));
            case ';': return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, strdup(";")));
            case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, strdup("(")));
            case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, strdup(")")));
            case '[': return lexer_advance_with_token(lexer, init_token(TOKEN_LSPAREN, strdup("[")));
            case ']': return lexer_advance_with_token(lexer, init_token(TOKEN_RSPAREN, strdup("]")));
            case '{': return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACKETS, strdup("{")));
            case '}': return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACKETS, strdup("}")));
            case '>': return lexer_advance_with_token(lexer, init_token(TOKEN_BIGGER, strdup(">")));
            case '<': return lexer_advance_with_token(lexer, init_token(TOKEN_LESS, strdup("<")));
            case ',': return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, strdup(",")));
        }

        lexer_advance(lexer);
    }
    return NULL;
}

token_T* lexer_collect_string_value(lexer_T* lexer) {
    lexer_advance(lexer);

    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while(lexer->c != '"'){
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, strlen(value) + strlen(s) + 1 * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_STRING_VALUE, value);
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token) {
    lexer_advance(lexer);
    return token;
}

char* lexer_get_current_char_as_string(lexer_T* lexer) {
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}