int my_min(int a, int b) {
    if (a < b) return a;
    return b;
}

int my_max(int a, int b) {
    if (a > b) return a;
    return b;
}

int main(void) {
    my_min(3, 7);
    my_max(3, 7);
    return 0;
}
