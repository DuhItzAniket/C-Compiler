int power(int base, int exp) {
    int r = 1;
    for (int i = 0; i < exp; i++) r = r * base;
    return r;
}

int main(void) {
    power(2, 5);
    return 0;
}
