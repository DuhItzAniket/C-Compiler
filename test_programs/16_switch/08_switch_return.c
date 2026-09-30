int classify(int x) {
    switch (x) {
        case 0:
            return 0;
        case 1:
            return 10;
        default:
            return 100;
    }
}

int main(void) {
    classify(1);
    return 0;
}
