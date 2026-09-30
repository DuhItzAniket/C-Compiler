int counter(void) {
    static int count = 0;
    count++;
    return count;
}

int main(void) {
    counter();
    counter();
    return 0;
}
