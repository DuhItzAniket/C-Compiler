int main(void) {
    int arr[5] = {5, 3, 1, 4, 2};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    return 0;
}