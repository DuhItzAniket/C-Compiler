void reverse(char s[]) {
    int len = 0;
    while (s[len] != 0) len++;
    for (int i = 0; i < len / 2; i++) {
        char t = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = t;
    }
}

int main(void) {
    char s[] = "hello";
    reverse(s);
    return 0;
}
