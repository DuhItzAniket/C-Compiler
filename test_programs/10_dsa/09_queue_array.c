int queue[10];
int front = 0;
int rear = 0;

void enqueue(int val) {
    queue[rear++] = val;
}

int dequeue(void) {
    return queue[front++];
}

int main(void) {
    enqueue(1);
    enqueue(2);
    dequeue();
    return 0;
}