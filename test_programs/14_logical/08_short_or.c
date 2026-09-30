int check(int x) {
    return x > 0;
}

int main(void) {
    int r = 1 || check(0);
    return 0;
}
