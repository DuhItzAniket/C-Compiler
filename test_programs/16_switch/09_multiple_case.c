int main(void) {
    int x = 2;
    int y = 0;
    switch (x) {
        case 1:
        case 2:
        case 3:
            y = 1;
            break;
        default:
            y = 0;
    }
    return 0;
}
