#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>

parser_T* init_parser(lexer_T* lexer){
    parser_T* parser = calloc(1, sizeof(parser_T));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->next_token = lexer_get_next_token(lexer);
    return parser;
}

void parser_advance(parser_T* parser){
    parser->current_token = parser->next_token;
    parser->next_token = lexer_get_next_token(parser->lexer);
}

void parser_eat(parser_T* parser, int token_type){
    if(parser->current_token->type == token_type){
        parser_advance(parser);
    } else {
        printf("Parser error: expected token type %d but got %d\n", token_type, parser->current_token->type);
        exit(1);
    }
}

ast_T* parser_parse(parser_T* parser){
    return parse_statement(parser);
}

ast_T* parse_statement(parser_T* parser){
    int pcttype = parser->current_token->type;
    if(pcttype == TOKEN_INT || pcttype == TOKEN_FLOAT ||
    pcttype == TOKEN_CHAR || pcttype == TOKEN_DOUBLE ||
    pcttype == TOKEN_STRING || pcttype == TOKEN_BOOL){
        return parse_variable_declaration(parser);
    }

    return parse_expression(parser);
}

ast_T* parse_variable_declaration(parser_T* parser){
    token_T* type_token = parser->current_token;
    parser_eat(parser, type_token->type);

    token_T* id_token = parser->current_token;
    parser_eat(parser, TOKEN_ID);

    ast_T* left = init_ast_identifier(id_token);

    if(parser->current_token->type == TOKEN_EQUALS){
        parser_eat(parser, TOKEN_EQUALS);
        ast_T* right = parse_expression(parser);

        return init_ast_binary_op(left, init_token(TOKEN_EQUALS, "="), right);
    }

    parser_eat(parser, TOKEN_SEMI);
    return left;
}

ast_T* parse_expression(parser_T* parser){
    ast_T* left = parse_term(parser);
    int pcttype = parser->current_token->type;
    
    while(pcttype == TOKEN_BIGGER || pcttype == TOKEN_LESS ||
    pcttype == TOKEN_PLUS || pcttype == TOKEN_MINUS){
        token_T* op_token = parser->current_token;
        parser_advance(parser);
        ast_T* right = parse_term(parser);
        left = init_ast_binary_op(left, op_token, right);
    }

    return left;
}

ast_T* parse_term(parser_T* parser){
    ast_T* left = parse_factor(parser);
    int pcttype = parser->current_token->type;

    while(pcttype == TOKEN_MUL || pcttype == TOKEN_DIV){
        token_T* op_token = parser->current_token;
        parser_advance(parser);
        ast_T* right = parse_factor(parser);
        left = init_ast_binary_op(left, op_token, right);
    }

    return left;
}

ast_T* parse_factor(parser_T* parser){
    token_T* token = parser->current_token;

    if(token->type == TOKEN_ID){
        parser_eat(parser, TOKEN_ID);
        return init_ast_identifier(token);
    } else if(token->type == TOKEN_INT || token->type == TOKEN_STRING_VALUE){
        parser_eat(parser, token->type);
        return init_ast_literal(token);
    } else if (token->type == TOKEN_LPAREN){
        parser_eat(parser, TOKEN_LPAREN);
        ast_T* expr = parse_expression(parser);
        parser_eat(parser, TOKEN_RPAREN);
        return expr;
    }

    printf("Unexpected token: %s\n", token->value);
    exit(1);
}