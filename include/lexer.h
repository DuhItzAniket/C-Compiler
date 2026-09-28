#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NONE,
    TOKEN_IDENT,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_CHAR,
    TOKEN_STRING,

    /* Arithmetic */
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,
    TOKEN_PERCENT,

    /* Assignment */
    TOKEN_EQ,

    /* Comparison */
    TOKEN_EQEQ, TOKEN_NOTEQ, TOKEN_LT, TOKEN_GT,
    TOKEN_LTEQ, TOKEN_GTEQ,
    TOKEN_NOT,

    /* Punctuation */
    TOKEN_SEMICOLON, TOKEN_COMMA,
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_LBRACKET, TOKEN_RBRACKET,

    /* Keywords */
    TOKEN_IF, TOKEN_ELSE, TOKEN_INT, TOKEN_RETURN,
    TOKEN_VOID, TOKEN_WHILE, TOKEN_FOR,

    TOKEN_EOF
} token_type_t;

typedef struct token {
    token_type_t type;
    char *value;
    int line_num;
    struct token *next;
} token_t;

token_t *lexer_lex(const char *source, int *out_token_count);
void tokenizer_free(token_t *tokens);

#endif