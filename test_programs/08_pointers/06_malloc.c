void func(int *ptr) {
    *ptr = 20;
}

int main(void) {
    int x = 10;
    func(&x);
    return 0;
}