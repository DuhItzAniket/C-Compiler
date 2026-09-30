enum Color { RED, GREEN, BLUE };

int main(void) {
    enum Color c = GREEN;
    switch (c) {
        case RED:
            return 1;
        case GREEN:
            return 2;
        case BLUE:
            return 3;
    }
    return 0;
}
