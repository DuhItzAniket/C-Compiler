#include <stdio.h>

int main(void) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j <= i; j++) {
            int v = 1;
            for (int k = 0; k < j; k++) v = v * (i - k) / (k + 1);
            printf("%d ", v);
        }
        printf("\n");
    }
    return 0;
}
