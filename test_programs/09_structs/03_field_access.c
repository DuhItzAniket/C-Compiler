struct Point {
    int x;
    int y;
};

int main(void) {
    struct Point p = {10, 20};
    p.x = 30;
    return 0;
}