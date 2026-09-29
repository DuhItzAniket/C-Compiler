int global = 100;

int get_global(void) {
    return global;
}

int main(void) {
    get_global();
    return 0;
}