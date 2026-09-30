int main(void) {
    int x = 10;
    {
        int x = 20;
        x = x + 1;
    }
    return 0;
}
