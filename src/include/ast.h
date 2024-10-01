#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum {
    AST_VARIABLE_DECLARATION,
    AST_ASSIGNMENT,
    AST_BINARY_OP,
    AST_LITERAL,
    AST_IDENTIFIER
} ast_node_type;

typedef struct AST_STRUCT {
    ast_node_type type;
    struct AST_STRUCT* left;
    struct AST_STRUCT* right;
    token_T* token;
    char* value;
} ast_T;

ast_T* init_ast_node(ast_node_type type);
ast_T* init_ast_literal(token_T* token);
ast_T* init_ast_identifier(token_T* token);
ast_T* init_ast_binary_op(ast_T* left, token_T* op_token, ast_T* right);

#endif