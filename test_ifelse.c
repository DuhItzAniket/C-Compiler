/* Test: simple if-else with comparison */
int main(void) {
    int a = 5;
    int b = 3;
    int c;
    
    if (a > b) {
        c = a - b;
    } else {
        c = b - a;
    }
    
    return c;
}