struct Inner {
    int a;
};

struct Outer {
    struct Inner inner;
};

int main(void) {
    struct Outer o = {{10}};
    return 0;
}