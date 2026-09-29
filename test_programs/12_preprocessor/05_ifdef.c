#define DEBUG 1

int main(void) {
    #ifdef DEBUG
    return 1;
    #else
    return 0;
    #endif
}