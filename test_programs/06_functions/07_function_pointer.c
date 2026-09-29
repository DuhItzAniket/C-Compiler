int add(int a, int b) {
    return a + b;
}

int main(void) {
    int (*fp)(int, int) = add;
    fp(5, 3);
    return 0;
}