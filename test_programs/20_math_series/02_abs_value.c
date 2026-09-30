int my_abs(int x) {
    if (x < 0) return 0 - x;
    return x;
}

int main(void) {
    my_abs(0 - 7);
    my_abs(7);
    return 0;
}
