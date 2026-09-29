struct Point {
    int x;
    int y;
};

struct Point create_point(int x, int y) {
    struct Point p = {x, y};
    return p;
}

int main(void) {
    struct Point p = create_point(10, 20);
    return 0;
}