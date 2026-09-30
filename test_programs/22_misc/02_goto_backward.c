int main(void) {
    int i = 0;
start:
    i++;
    if (i < 3) goto start;
    return 0;
}
