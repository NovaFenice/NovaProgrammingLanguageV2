#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct PARSER_STRUCT {
    lexer_T* lexer;
    token_T* current_token;
    token_T* next_token;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_advance(parser_T* parser);
void parser_eat(parser_T* parser, int token_type);

ast_T* parser_parse(parser_T* parser);
ast_T* parse_statement(parser_T* parser);
ast_T* parse_variable_declaration(parser_T* parser);
ast_T* parse_expression(parser_T* parser);
ast_T* parse_term(parser_T* parser);
ast_T* parse_factor(parser_T* parser);

#endif