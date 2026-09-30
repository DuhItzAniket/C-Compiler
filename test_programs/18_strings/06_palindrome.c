int is_palindrome(char s[]) {
    int len = 0;
    while (s[len] != 0) len++;
    for (int i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) return 0;
    }
    return 1;
}

int main(void) {
    char s[] = "madam";
    is_palindrome(s);
    return 0;
}
