int my_strcmp(char a[], char b[]) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

int main(void) {
    char x[] = "abc";
    char y[] = "abd";
    my_strcmp(x, y);
    return 0;
}
