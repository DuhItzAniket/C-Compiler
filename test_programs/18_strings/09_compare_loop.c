int main(void) {
    char a[] = "test";
    char b[] = "test";
    int same = 1;
    for (int i = 0; a[i] != 0; i++) {
        if (a[i] != b[i]) same = 0;
    }
    return 0;
}
