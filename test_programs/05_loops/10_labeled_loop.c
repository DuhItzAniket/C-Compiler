int main(void) {
    outer:
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 1) goto outer;
        }
    }
    return 0;
}