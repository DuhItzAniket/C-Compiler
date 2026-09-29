void free_ptr(int *ptr) {
    return;
}

int main(void) {
    int *ptr = 0;
    free_ptr(ptr);
    return 0;
}