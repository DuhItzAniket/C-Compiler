#include "parser.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    token_t *tokens;
    int count;
    int pos;
} ParserState;

static ParserState ps;

static void ps_advance(void) { ps.pos++; }

static token_type_t ps_peek_type(void) {
    if (ps.pos >= ps.count) return TOKEN_EOF;
    return ps.tokens[ps.pos].type;
}

static int ps_peek_line(void) {
    if (ps.pos >= ps.count) return 0;
    return ps.tokens[ps.pos].line_num;
}

static int ps_match(token_type_t type) {
    if (ps_peek_type() == type) {
        ps_advance();
        return 1;
    }
    return 0;
}

static ast_node_t *ps_make_node(ast_node_type_t type) {
    ast_node_t *node = (ast_node_t *)calloc(1, sizeof(ast_node_t));
    if (!node) return NULL;
    node->type = type;
    node->line_num = ps_peek_line();
    return node;
}

static ast_node_t *ps_parse_primary(void) {
    token_type_t t = ps_peek_type();

    if (t == TOKEN_INTEGER) {
        ps_advance();
        ast_node_t *node = ps_make_node(AST_INTEGER);
        if (node && ps.pos > 0 && ps.tokens[ps.pos - 1].value)
            node->ival = atoi(ps.tokens[ps.pos - 1].value);
        return node;
    }

    if (t == TOKEN_IDENT) {
        ps_advance();
        ast_node_t *node = ps_make_node(AST_IDENT);
        if (node && ps.pos > 0 && ps.tokens[ps.pos - 1].value) {
            node->name = strdup(ps.tokens[ps.pos - 1].value);
        }
        return node;
    }

    if (t == TOKEN_STRING) {
        ps_advance();
        ast_node_t *node = ps_make_node(AST_STRING);
        if (node && ps.pos > 0 && ps.tokens[ps.pos - 1].value) {
            node->sval = strdup(ps.tokens[ps.pos - 1].value);
        }
        return node;
    }

    if (t == TOKEN_LPAREN) {
        ps_advance();
        ast_node_t *node = ps_parse_expr(0);
        if (node && ps_match(TOKEN_RPAREN)) {
            return node;
        }
        return NULL;
    }

    return NULL;
}

static ast_node_t *ps_parse_unary(void) {
    token_type_t t = ps_peek_type();

    if (t == TOKEN_MINUS || t == TOKEN_NOT) {
        ps_advance();
        ast_node_t *node = ps_make_node(AST_UNARY);
        node->children[0] = ps_parse_unary();
        return node;
    }

    return ps_parse_primary();
}

static ast_node_t *ps_parse_factor(void) {
    ast_node_t *left = ps_parse_unary();

    while (ps_peek_type() == TOKEN_STAR || ps_peek_type() == TOKEN_SLASH ||
           ps_peek_type() == TOKEN_PERCENT) {
        token_type_t op = ps_peek_type();
        ps_advance();
        ast_node_t *right = ps_parse_unary();
        if (!left || !right) return NULL;

        ast_node_t *node = ps_make_node(AST_BINARY);
        node->children[0] = left;
        node->children[1] = right;
        /* store operator - use a simple encoding */
        left = node;
    }
    return left;
}

static ast_node_t *ps_parse_term(void) {
    ast_node_t *left = ps_parse_factor();

    while (ps_peek_type() == TOKEN_PLUS || ps_peek_type() == TOKEN_MINUS) {
        token_type_t op = ps_peek_type();
        ps_advance();
        ast_node_t *right = ps_parse_factor();
        if (!left || !right) return NULL;

        ast_node_t *node = ps_make_node(AST_BINARY);
        node->children[0] = left;
        node->children[1] = right;
        left = node;
    }
    return left;
}

static ast_node_t *ps_parse_comparison(void) {
    ast_node_t *left = ps_parse_term();

    while (ps_peek_type() == TOKEN_LT || ps_peek_type() == TOKEN_GT ||
           ps_peek_type() == TOKEN_LTEQ || ps_peek_type() == TOKEN_GTEQ) {
        token_type_t op = ps_peek_type();
        ps_advance();
        ast_node_t *right = ps_parse_term();
        if (!left || !right) return NULL;

        ast_node_t *node = ps_make_node(AST_BINARY);
        node->children[0] = left;
        node->children[1] = right;
        left = node;
    }
    return left;
}

static ast_node_t *ps_parse_equality(void) {
    ast_node_t *left = ps_parse_comparison();

    while (ps_peek_type() == TOKEN_EQEQ || ps_peek_type() == TOKEN_NOTEQ) {
        token_type_t op = ps_peek_type();
        ps_advance();
        ast_node_t *right = ps_parse_comparison();
        if (!left || !right) return NULL;

        ast_node_t *node = ps_make_node(AST_BINARY);
        node->children[0] = left;
        node->children[1] = right;
        left = node;
    }
    return left;
}

static ast_node_t *ps_parse_expr(void) {
    return ps_parse_equality();
}

static ast_node_t *ps_parse_declaration(void);
static ast_node_t *ps_parse_statement(void);

static ast_node_t *ps_parse_var_decl(void) {
    /* type_spec ident ';' */
    ast_node_t *node = ps_make_node(AST_DECL);
    if (!node) return NULL;

    /* type is int or void - store simply */
    node->children[0] = ps_make_node(/* type marker */);

    if (!ps_match(TOKEN_IDENT)) {
        ps_advance(); /* recover */
        return NULL;
    }
    node->name = strdup(ps.tokens[ps.pos - 1].value);

    if (!ps_match(TOKEN_SEMICOLON)) {
        /* error - expect ; */
        return NULL;
    }
    return node;
}

static ast_node_t *ps_parse_assignment(void) {
    /* ident '=' expr ';' */
    if (ps_peek_type() != TOKEN_IDENT) return ps_parse_expr(0);

    ps_advance();
    if (!ps_match(TOKEN_EQ)) {
        /* not an assignment, put ident back */
        return ps_parse_expr(0);
    }

    ast_node_t *node = ps_make_node(AST_ASSIGN);
    if (!node) return NULL;
    node->name = strdup(ps.tokens[ps.pos - 2].value); /* the ident */
    node->children[0] = ps_parse_expr(0);
    if (!ps_match(TOKEN_SEMICOLON)) return NULL;
    return node;
}

static ast_node_t *ps_parse_statement(void) {
    token_type_t t = ps_peek_type();

    if (t == TOKEN_IF) {
        ps_advance();
        if (!ps_match(TOKEN_LPAREN)) return NULL;
        ast_node_t *cond = ps_parse_expr(0);
        if (!ps_match(TOKEN_RPAREN)) return NULL;
        ast_node_t *then_stmt = ps_parse_statement();
        if (!then_stmt) return NULL;

        ast_node_t *node = ps_make_node(AST_IF);
        if (!node) return NULL;
        node->children[0] = cond;
        node->children[1] = then_stmt;

        if (ps_match(TOKEN_ELSE)) {
            node->children[2] = ps_parse_statement();
        }
        return node;
    }

    if (t == TOKEN_WHILE) {
        ps_advance();
        if (!ps_match(TOKEN_LPAREN)) return NULL;
        ast_node_t *cond = ps_parse_expr(0);
        if (!ps_match(TOKEN_RPAREN)) return NULL;
        ast_node_t *body = ps_parse_statement();
        if (!body) return NULL;

        ast_node_t *node = ps_make_node(AST_WHILE);
        if (!node) return NULL;
        node->children[0] = cond;
        node->children[1] = body;
        return node;
    }

    if (t == TOKEN_FOR) {
        ps_advance();
        /* simplified: for(expr;expr;expr) */
        if (!ps_match(TOKEN_LPAREN)) return NULL;
        ast_node_t *init = ps_parse_expr(0);
        if (!ps_match(TOKEN_SEMICOLON)) return NULL;
        ast_node_t *cond = ps_parse_expr(0);
        if (!ps_match(TOKEN_SEMICOLON)) return NULL;
        ast_node_t *inc = ps_parse_expr(0);
        if (!ps_match(TOKEN_RPAREN)) return NULL;
        ast_node_t *body = ps_parse_statement();
        if (!body) return NULL;

        ast_node_t *node = ps_make_node(AST_FOR);
        if (!node) return NULL;
        node->children[0] = init;
        node->children[1] = cond;
        node->children[2] = inc;
        node->children[3] = body;
        return node;
    }

    if (t == TOKEN_RETURN) {
        ps_advance();
        ast_node_t *val = ps_parse_expr(0);
        if (!ps_match(TOKEN_SEMICOLON)) return NULL;
        ast_node_t *node = ps_make_node(AST_RETURN);
        if (!node) return NULL;
        node->children[0] = val;
        return node;
    }

    if (t == TOKEN_LBRACE) {
        ps_advance();
        ast_node_t *body = NULL;
        ast_node_t **tail = &body;
        while (ps_peek_type() != TOKEN_RBRACE && ps_peek_type() != TOKEN_EOF) {
            *tail = ps_parse_statement();
            if (*tail) tail = &(*tail)->children[0]; /* link list */
        }
        if (ps_match(TOKEN_RBRACE)) {
            return body;
        }
        return NULL;
    }

    /* expression statement or var_decl */
    return ps_parse_assignment();
}

ast_node_t *parser_parse(token_t *tokens, int token_count) {
    ps.tokens = tokens;
    ps.count = token_count;
    ps.pos = 0;

    ast_node_t *prog = ps_make_node(AST_PROGRAM);
    if (!prog) return NULL;

    while (ps_peek_type() != TOKEN_EOF && ps_peek_type() != TOKEN_NONE) {
        ast_node_t *decl = ps_parse_declaration();
        if (decl) {
            /* link to program list */
            decl->children[0] = prog->children[0];
            prog->children[0] = decl;
        }
    }
    return prog;
}

ast_node_t *parser_parse_declaration(void) {
    /* simplified: handle func_decl or var_decl */
    if (ps_match(TOKEN_INT) || ps_match(TOKEN_VOID) || ps_match(TOKEN_IF) ||
        ps_match(TOKEN_WHILE) || ps_match(TOKEN_FOR) || ps_match(TOKEN_RETURN)) {
        return ps_parse_statement();
    }
    /* maybe a var_decl starting with ident lookahead */
    return ps_parse_var_decl();
}

void ast_free(ast_node_t *node) {
    if (!node) return;
    for (int i = 0; i < 4; i++) {
        if (node->children[i]) {
            ast_free(node->children[i]);
        }
    }
    free(node->name);
    free(node->sval);
    free(node);
}

void ast_print(ast_node_t *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type) {
        case AST_PROGRAM: printf("PROGRAM\n"); break;
        case AST_DECL: printf("DECL %s\n", node->name ? node->name : "?"); break;
        case AST_INTEGER: printf("INTEGER %d\n", node->ival); break;
        case AST_IDENT: printf("IDENT %s\n", node->name ? node->name : "?"); break;
        case AST_STRING: printf("STRING %s\n", node->sval ? node->sval : "?"); break;
        case AST_BINARY: printf("BINARY op\n"); break;
        case AST_UNARY: printf("UNARY\n"); break;
        case AST_RETURN: printf("RETURN\n"); break;
        case AST_IF: printf("IF\n"); break;
        case AST_WHILE: printf("WHILE\n"); break;
        case AST_FOR: printf("FOR\n"); break;
        default: printf("NODE(type=%d)\n", node->type);
    }
    for (int i = 0; i < 4 && node->children[i]; i++) {
        ast_print(node->children[i], indent + 1);
    }
}