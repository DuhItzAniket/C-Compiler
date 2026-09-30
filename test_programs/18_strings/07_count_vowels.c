int count_vowels(char s[]) {
    int c = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u') c++;
        i++;
    }
    return c;
}

int main(void) {
    char s[] = "hello world";
    count_vowels(s);
    return 0;
}
