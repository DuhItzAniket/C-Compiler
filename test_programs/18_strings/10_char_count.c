int count_char(char s[], char target) {
    int c = 0;
    for (int i = 0; s[i] != 0; i++) {
        if (s[i] == target) c++;
    }
    return c;
}

int main(void) {
    char s[] = "banana";
    count_char(s, 'a');
    return 0;
}
