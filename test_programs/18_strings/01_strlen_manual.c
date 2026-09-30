int my_strlen(char s[]) {
    int len = 0;
    while (s[len] != 0) {
        len++;
    }
    return len;
}

int main(void) {
    char s[] = "hello";
    my_strlen(s);
    return 0;
}
