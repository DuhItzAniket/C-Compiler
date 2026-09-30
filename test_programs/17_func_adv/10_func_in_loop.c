int square(int x) {
    return x * x;
}

int main(void) {
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total = total + square(i);
    }
    return 0;
}
