int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return 0 - 1;
    return 0;
}

int main(void) {
    sign(5);
    sign(0 - 5);
    return 0;
}
