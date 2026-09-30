void to_upper(char s[]) {
    int i = 0;
    while (s[i] != 0) {
        if (s[i] >= 'a' && s[i] <= 'z') s[i] = s[i] - 32;
        i++;
    }
}

int main(void) {
    char s[] = "hello";
    to_upper(s);
    return 0;
}
