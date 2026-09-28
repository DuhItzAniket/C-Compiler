#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char *keywords[] = {
    "if", "else", "int", "return", "void", "while", "for"
};

static token_type_t keyword_lookup(const char *str) {
    for (int i = 0; i < 7; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            switch (i) {
                case 0: return TOKEN_IF;
                case 1: return TOKEN_ELSE;
                case 2: return TOKEN_INT;
                case 3: return TOKEN_RETURN;
                case 4: return TOKEN_VOID;
                case 5: return TOKEN_WHILE;
                case 6: return TOKEN_FOR;
                default: return TOKEN_IDENT;
            }
        }
    }
    return TOKEN_IDENT;
}

token_t *lexer_lex(const char *source, int *out_token_count) {
    token_t head = {0};
    token_t *tail = &head;
    int capacity = 64;
    int count = 0;
    token_t *tokens = calloc(capacity, sizeof(token_t));

    if (!tokens) return NULL;

    int i = 0;
    int len = (int)strlen(source);
    int line_num = 1;

    while (i < len) {
        char c = source[i];

        /* Skip whitespace */
        if (isspace((unsigned char)c)) {
            if (c == '\n') line_num++;
            i++;
            continue;
        }

        /* Comments: // */
        if (c == '/' && i + 1 < len && source[i + 1] == '/') {
            while (i < len && source[i] != '\n') i++;
            continue;
        }

        /* Comments: /* */ */
        if (c == '/' && i + 1 < len && source[i + 1] == '*') {
            i += 2;
            while (i + 1 < len && !(source[i] == '*' && source[i + 1] == '/')) {
                if (source[i] == '\n') line_num++;
                i++;
            }
            i += 2; /* skip */ */
            continue;
        }

        /* Number: integer or float */
        if (isdigit((unsigned char)c)) {
            int start = i;
            int has_dot = 0;
            while (i < len && (isdigit((unsigned char)source[i]) || source[i] == '.')) {
                if (source[i] == '.') {
                    if (has_dot) break;
                    has_dot = 1;
                }
                i++;
            }
            int token_len = i - start;
            tokens[count].value = malloc(token_len + 1);
            strncpy(tokens[count].value, source + start, token_len);
            tokens[count].value[token_len] = '\0';
            tokens[count].line_num = line_num;

            if (has_dot) {
                tokens[count].type = TOKEN_FLOAT;
            } else {
                tokens[count].type = TOKEN_INTEGER;
            }
            count++;

            if (count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(token_t));
            }
            continue;
        }

        /* String literal */
        if (c == '"') {
            i++; /* skip opening quote */
            int start = i;
            while (i < len && source[i] != '"') {
                if (source[i] == '\n') line_num++;
                i++;
            }
            int token_len = i - start;
            tokens[count].value = malloc(token_len + 1);
            strncpy(tokens[count].value, source + start, token_len);
            tokens[count].value[token_len] = '\0';
            tokens[count].type = TOKEN_STRING;
            tokens[count].line_num = line_num;
            count++;

            if (i < len) i++; /* skip closing quote */
            if (count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(token_t));
            }
            continue;
        }

        /* Identifiers and keywords */
        if (isalpha((unsigned char)c) || c == '_') {
            int start = i;
            while (i < len && (isalnum((unsigned char)source[i]) || source[i] == '_')) {
                i++;
            }
            int token_len = i - start;
            tokens[count].value = malloc(token_len + 1);
            strncpy(tokens[count].value, source + start, token_len);
            tokens[count].value[token_len] = '\0';
            tokens[count].type = keyword_lookup(tokens[count].value);
            tokens[count].line_num = line_num;
            count++;

            if (count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(token_t));
            }
            continue;
        }

        /* Operators and punctuation - single char */
        switch (c) {
            case '+': tokens[count].type = TOKEN_PLUS; break;
            case '-': tokens[count].type = TOKEN_MINUS; break;
            case '*': tokens[count].type = TOKEN_STAR; break;
            case '/': tokens[count].type = TOKEN_SLASH; break;
            case '%': tokens[count].type = TOKEN_PERCENT; break;
            case '=': tokens[count].type = TOKEN_EQ; break;
            case ';': tokens[count].type = TOKEN_SEMICOLON; break;
            case ',': tokens[count].type = TOKEN_COMMA; break;
            case '(': tokens[count].type = TOKEN_LPAREN; break;
            case ')': tokens[count].type = TOKEN_RPAREN; break;
            case '{': tokens[count].type = TOKEN_LBRACE; break;
            case '}': tokens[count].type = TOKEN_RBRACE; break;
            case '[': tokens[count].type = TOKEN_LBRACKET; break;
            case ']': tokens[count].type = TOKEN_RBRACKET; break;
            case '<': tokens[count].type = TOKEN_LT; break;
            case '>': tokens[count].type = TOKEN_GT; break;
            case '!': tokens[count].type = TOKEN_NOT; break;
            default:
                /* Unknown character - error */
                free(tokens[count].value);
                free(tokens);
                *out_token_count = 0;
                return NULL;
        }
        tokens[count].value = NULL;
        tokens[count].line_num = line_num;
        count++;

        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(token_t));
        }
        i++;
    }

    /* Add EOF token */
    tokens[count].type = TOKEN_EOF;
    tokens[count].value = NULL;
    tokens[count].line_num = line_num;
    count++;

    *out_token_count = count;
    return tokens;
}

void tokenizer_free(token_t *tokens) {
    token_t *curr = tokens;
    while (curr) {
        token_t *next = curr->next;
        free(curr->value);
        free(curr);
        curr = next;
    }
}