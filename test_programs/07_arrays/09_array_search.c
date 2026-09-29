int main(void) {
    int arr[5] = {1, 2, 3, 4, 5};
    int target = 3;
    for (int i = 0; i < 5; i++) {
        if (arr[i] == target) break;
    }
    return 0;
}