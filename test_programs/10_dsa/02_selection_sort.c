int main(void) {
    int arr[5] = {5, 3, 1, 4, 2};
    for (int i = 0; i < 4; i++) {
        int min_idx = i;
        for (int j = i + 1; j < 5; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        int temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
    return 0;
}