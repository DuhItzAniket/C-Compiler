#include <stdio.h>
#include "lexer.h"

int main(void) {
    const char *source = "int main() { return 0; }";
    int count;
    token_t *tokens = lexer_lex(source, &count);

    if (!tokens) {
        printf("Lexer failed!\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("Token %d: type=%d, value=%s, line=%d\n",
               i, tokens[i].type, tokens[i].value ? tokens[i].value : "(none)", tokens[i].line_num);
    }

    tokenizer_free(tokens);
    printf("Lexer test passed! %d tokens generated.\n", count);
    return 0;
}