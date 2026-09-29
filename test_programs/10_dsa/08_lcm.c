int lcm(int a, int b) {
    int g = a;
    int h = b;
    while (h != 0) {
        int t = h;
        h = g % h;
        g = t;
    }
    return (a * b) / g;
}

int main(void) {
    lcm(12, 18);
    return 0;
}