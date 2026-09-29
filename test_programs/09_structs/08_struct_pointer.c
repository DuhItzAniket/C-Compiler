struct Point {
    int x;
    int y;
};

int main(void) {
    struct Point p = {10, 20};
    struct Point *ptr = &p;
    return 0;
}