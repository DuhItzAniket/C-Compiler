int main(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int sum = 0;
    for (int i = 0; i < 5; i++) sum = sum + arr[i];
    int avg = sum / 5;
    return 0;
}
