int stack[10];
int top = -1;

void push(int val) {
    stack[++top] = val;
}

int pop(void) {
    return stack[top--];
}

int main(void) {
    push(1);
    push(2);
    pop();
    return 0;
}