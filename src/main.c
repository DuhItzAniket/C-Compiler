#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    /* Read source file */
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Error: cannot open file %s\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *source = malloc(fsize + 1);
    fread(source, 1, fsize, f);
    source[fsize] = '\0';
    fclose(f);

    /* Lexical analysis */
    int token_count;
    token_t *tokens = lexer_lex(source, &token_count);
    if (!tokens) {
        printf("Lexer failed!\n");
        free(source);
        return 1;
    }

    /* Parse */
    ast_node_t *ast = parser_parse(tokens, token_count);
    if (!ast) {
        printf("Parser failed!\n");
        tokenizer_free(tokens);
        free(source);
        return 1;
    }

    /* Print AST for debugging */
    printf("Abstract Syntax Tree:\n");
    ast_print(ast, 0);

    /* Generate x86-64 assembly */
    codegen_generate(ast, "output.s");

    /* Cleanup */
    ast_free(ast);
    tokenizer_free(tokens);
    free(source);

    printf("\nCompilation complete. Output: output.s\n");
    return 0;
}