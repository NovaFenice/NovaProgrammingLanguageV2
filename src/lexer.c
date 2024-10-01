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

int is_keyword(const char* str) {
    return strcmp(str, "int") == 0 || strcmp(str, "float") == 0 || strcmp(str, "double") == 0 ||
           strcmp(str, "char") == 0 || strcmp(str, "string") == 0 || strcmp(str, "bool") == 0;
}

int get_keyword_token_type(const char* str) {
    if (strcmp(str, "int") == 0) return TOKEN_INT;
    if (strcmp(str, "float") == 0) return TOKEN_FLOAT;
    if (strcmp(str, "double") == 0) return TOKEN_DOUBLE;
    if (strcmp(str, "char") == 0) return TOKEN_CHAR;
    if (strcmp(str, "string") == 0) return TOKEN_STRING;
    if (strcmp(str, "bool") == 0) return TOKEN_BOOL;
    return TOKEN_ID;
}

token_T* lexer_collect_id(lexer_T* lexer) {
    char buffer[BUFFER_SIZE];
    int len = 0;

    while (isalnum(lexer->c) && len < BUFFER_SIZE - 1) {
        buffer[len++] = lexer->c;
        lexer_advance(lexer);
    }
    buffer[len] = '\0';

    if (is_keyword(buffer)) {
        return init_token(get_keyword_token_type(buffer), strdup(buffer));
    }

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
            case '=': return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, "="));
            case ';': return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, ";"));
            case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, "("));
            case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, ")"));
            case '[': return lexer_advance_with_token(lexer, init_token(TOKEN_LSPAREN, "["));
            case ']': return lexer_advance_with_token(lexer, init_token(TOKEN_RSPAREN, "]"));
            case '{': return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACKETS, "{"));
            case '}': return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACKETS, "}"));
            case '>': return lexer_advance_with_token(lexer, init_token(TOKEN_BIGGER, ">"));
            case '<': return lexer_advance_with_token(lexer, init_token(TOKEN_LESS, "<"));
            case ',': return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, ","));
            case '!': return lexer_advance_with_token(lexer, init_token(TOKEN_NOT, "!"));
            case '+': return lexer_advance_with_token(lexer, init_token(TOKEN_PLUS, "+"));
            case '-': return lexer_advance_with_token(lexer, init_token(TOKEN_MINUS, "-"));
            case '*': return lexer_advance_with_token(lexer, init_token(TOKEN_MUL, "*"));
            case '/': return lexer_advance_with_token(lexer, init_token(TOKEN_DIV, "/"));
        }

        lexer_advance(lexer);
    }
    return init_token(TOKEN_E0F, "");
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