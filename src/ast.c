#include "include/ast.h"
#include <stdlib.h>

ast_T* init_ast_node(ast_node_type type){
    ast_T* ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;
    return ast;
}

ast_T* init_ast_literal(token_T* token){
    ast_T* ast = init_ast_node(AST_LITERAL);
    ast->token = token;
    ast->value = token->value;
    return ast;
}

ast_T* init_ast_identifier(token_T* token){
    ast_T* ast = init_ast_node(AST_IDENTIFIER);
    ast->token = token;
    ast->value = token->value;
    return ast;
}

ast_T* init_ast_binary_op(ast_T* left, token_T* op_token, ast_T* right){
    ast_T* ast = init_ast_node(AST_BINARY_OP);
    ast->left = left;
    ast->token = op_token;
    ast->right = right;
    return ast;
}