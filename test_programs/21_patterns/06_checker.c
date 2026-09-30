#include <stdio.h>

int main(void) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((i + j) % 2 == 0) printf("*");
            else printf(" ");
        }
        printf("\n");
    }
    return 0;
}
