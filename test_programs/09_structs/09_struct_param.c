struct Point {
    int x;
    int y;
};

void print_point(struct Point p) {
    return;
}

int main(void) {
    struct Point p = {10, 20};
    print_point(p);
    return 0;
}