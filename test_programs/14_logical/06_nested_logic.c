int main(void) {
    int a = 1;
    int b = 1;
    int c = 0;
    int r = a && (b || c) && !(a && c);
    return 0;
}
