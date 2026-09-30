int main(void) {
    int x = 0;
    goto skip;
    x = 100;
skip:
    x = 1;
    return 0;
}
