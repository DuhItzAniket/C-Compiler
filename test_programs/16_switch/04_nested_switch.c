int main(void) {
    int a = 1;
    int b = 2;
    switch (a) {
        case 1:
            switch (b) {
                case 2:
                    return 1;
            }
            break;
    }
    return 0;
}
