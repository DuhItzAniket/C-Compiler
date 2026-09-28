#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    AST_PROGRAM,
    AST_FUNC,
    AST_DECL,
    AST_ASSIGN,
    AST_RETURN,
    AST_BINARY,
    AST_UNARY,
    AST_IDENT,
    AST_INTEGER,
    AST_STRING,
    AST_CALL,
    AST_IF,
    AST_WHILE,
} ast_node_type_t;

typedef struct ast_node {
    ast_node_type_t type;
    int line_num;
    struct ast_node *children[3]; /* left, right, extra */
    char *name;                   /* for IDENT, FUNCTION names */
    int ival;                     /* for INTEGER values */
    char *sval;                   /* for STRING values */
} ast_node_t;

ast_node_t *parser_parse(token_t *tokens, int token_count);
void ast_free(ast_node_t *node);
void ast_print(ast_node_t *node, int indent);

#endif