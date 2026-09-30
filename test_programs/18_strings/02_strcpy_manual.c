void my_strcpy(char d[], char s[]) {
    int i = 0;
    while (s[i] != 0) {
        d[i] = s[i];
        i++;
    }
    d[i] = 0;
}

int main(void) {
    char a[10];
    char b[] = "hi";
    my_strcpy(a, b);
    return 0;
}
