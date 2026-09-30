void my_strcat(char d[], char s[]) {
    int i = 0;
    while (d[i] != 0) i++;
    int j = 0;
    while (s[j] != 0) {
        d[i] = s[j];
        i++;
        j++;
    }
    d[i] = 0;
}

int main(void) {
    char buf[20] = "hi ";
    char extra[] = "there";
    my_strcat(buf, extra);
    return 0;
}
