#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    int data[MAX];
    int top;
} Stack;

void initStack(Stack *s) { s->top = -1; }
int isEmpty(Stack *s) { return s->top == -1; }
void push(Stack *s, int x) { s->data[++s->top] = x; }
int pop(Stack *s) { return s->data[s->top--]; }
int peek(Stack *s) { return s->data[s->top]; }

void printStack(const char *label, Stack *s) {
    printf("%s (bottom -> top): ", label);
    for (int i = 0; i <= s->top; i++) printf("%d ", s->data[i]);
    printf("\n");
}

/* Q9. Convert a decimal number to binary using a stack. */
void decimalToBinary(int n) {
    Stack s;
    initStack(&s);
    if (n == 0) push(&s, 0);
    while (n > 0) {
        push(&s, n % 2);
        n /= 2;
    }
    while (!isEmpty(&s)) printf("%d", pop(&s));
    printf("\n");
}

/* Q10. Check whether a string is a palindrome using a stack. */
int isPalindrome(const char *str) {
    Stack s;
    initStack(&s);
    int len = strlen(str);
    for (int i = 0; i < len; i++) push(&s, str[i]);
    for (int i = 0; i < len; i++) {
        if (str[i] != pop(&s)) return 0;
    }
    return 1;
}

/* Q11. Delete the middle element of a stack. */
void deleteMid(Stack *s, int size, int current) {
    if (isEmpty(s) || current == size) return;
    int x = pop(s);
    deleteMid(s, size, current + 1);
    if (current != size / 2) push(s, x);
}

/* Q12. Insert an element at the bottom of a stack using recursion. */
void insertAtBottom(Stack *s, int x) {
    if (isEmpty(s)) {
        push(s, x);
        return;
    }
    int top = pop(s);
    insertAtBottom(s, x);
    push(s, top);
}

/* Q13. Sort a stack using a temporary stack. */
void sortStack(Stack *s) {
    Stack tmp;
    initStack(&tmp);
    while (!isEmpty(s)) {
        int x = pop(s);
        while (!isEmpty(&tmp) && peek(&tmp) > x) push(s, pop(&tmp));
        push(&tmp, x);
    }
    *s = tmp;
}

/* Q14. Implement two stacks in a single array (space optimization) with push and pop for both. */
typedef struct {
    int arr[MAX];
    int size;
    int top1;
    int top2;
} TwoStacks;

void initTwoStacks(TwoStacks *t, int size) {
    t->size = size;
    t->top1 = -1;
    t->top2 = size;
}

void push1(TwoStacks *t, int x) {
    if (t->top1 + 1 == t->top2) {
        printf("Stack Overflow\n");
        return;
    }
    t->arr[++t->top1] = x;
}

void push2(TwoStacks *t, int x) {
    if (t->top1 + 1 == t->top2) {
        printf("Stack Overflow\n");
        return;
    }
    t->arr[--t->top2] = x;
}

int pop1(TwoStacks *t) {
    if (t->top1 == -1) {
        printf("Stack 1 Underflow\n");
        return -1;
    }
    return t->arr[t->top1--];
}

int pop2(TwoStacks *t) {
    if (t->top2 == t->size) {
        printf("Stack 2 Underflow\n");
        return -1;
    }
    return t->arr[t->top2++];
}

/* Q15. Convert an infix expression to prefix, for example (A + B) * C - D, with a step-by-step table. */
int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

void printStackChars(Stack *s, char *buf) {
    int k = 0;
    for (int i = 0; i <= s->top; i++) buf[k++] = (char)s->data[i];
    buf[k] = '\0';
}

void infixToPrefix(const char *infix, char *prefix) {
    char rev[MAX];
    int n = 0;
    for (int i = strlen(infix) - 1; i >= 0; i--) {
        char c = infix[i];
        if (c == ' ') continue;
        if (c == '(') c = ')';
        else if (c == ')') c = '(';
        rev[n++] = c;
    }
    rev[n] = '\0';
    printf("Reversed (brackets swapped): %s\n", rev);

    Stack s;
    initStack(&s);
    char out[MAX];
    int k = 0;
    char stk[MAX];

    printf("%-8s %-12s %-12s\n", "Symbol", "Stack", "Output");
    for (int i = 0; i < n; i++) {
        char c = rev[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            out[k++] = c;
        } else if (c == '(') {
            push(&s, c);
        } else if (c == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') out[k++] = (char)pop(&s);
            pop(&s);
        } else {
            while (!isEmpty(&s) && precedence((char)peek(&s)) > precedence(c)) out[k++] = (char)pop(&s);
            push(&s, c);
        }
        out[k] = '\0';
        printStackChars(&s, stk);
        printf("%-8c %-12s %-12s\n", c, stk, out);
    }
    while (!isEmpty(&s)) {
        out[k++] = (char)pop(&s);
        out[k] = '\0';
        printStackChars(&s, stk);
        printf("%-8s %-12s %-12s\n", "(pop)", stk, out);
    }
    printf("Postfix of reversed: %s\n", out);

    for (int i = 0; i < k; i++) prefix[i] = out[k - 1 - i];
    prefix[k] = '\0';
}

/* Q16. Evaluate a prefix expression using a stack, for example - * + 4 3 2 5. */
int evaluatePrefix(const char *expr) {
    Stack s;
    initStack(&s);
    for (int i = strlen(expr) - 1; i >= 0; i--) {
        char c = expr[i];
        if (c == ' ') continue;
        if (c >= '0' && c <= '9') {
            push(&s, c - '0');
        } else {
            int a = pop(&s);
            int b = pop(&s);
            int r = 0;
            switch (c) {
                case '+': r = a + b; break;
                case '-': r = a - b; break;
                case '*': r = a * b; break;
                case '/': r = a / b; break;
            }
            push(&s, r);
        }
    }
    return pop(&s);
}

/* Q17. Solve the Stock Span problem using a stack in O(n). */
void stockSpan(int price[], int n, int span[]) {
    Stack s;
    initStack(&s);
    for (int i = 0; i < n; i++) {
        while (!isEmpty(&s) && price[peek(&s)] <= price[i]) pop(&s);
        span[i] = isEmpty(&s) ? i + 1 : i - peek(&s);
        push(&s, i);
    }
}

int main(void) {
    printf("Q9:\n");
    int num = 25;
    printf("%d in binary = ", num);
    decimalToBinary(num);
    printf("\n");

    printf("Q10:\n");
    const char *words[] = {"madam", "hello"};
    for (int i = 0; i < 2; i++)
        printf("%s -> %s\n", words[i], isPalindrome(words[i]) ? "Palindrome" : "Not a palindrome");
    printf("\n");

    printf("Q11:\n");
    Stack s11;
    initStack(&s11);
    for (int i = 1; i <= 5; i++) push(&s11, i);
    printStack("Before", &s11);
    deleteMid(&s11, s11.top + 1, 0);
    printStack("After", &s11);
    printf("\n");

    printf("Q12:\n");
    Stack s12;
    initStack(&s12);
    for (int i = 1; i <= 4; i++) push(&s12, i);
    printStack("Before", &s12);
    insertAtBottom(&s12, 99);
    printStack("After", &s12);
    printf("\n");

    printf("Q13:\n");
    Stack s13;
    initStack(&s13);
    int vals[] = {34, 3, 31, 98, 92, 23};
    for (int i = 0; i < 6; i++) push(&s13, vals[i]);
    printStack("Before", &s13);
    sortStack(&s13);
    printStack("After", &s13);
    printf("\n");

    printf("Q14:\n");
    TwoStacks t;
    initTwoStacks(&t, 6);
    push1(&t, 10);
    push1(&t, 20);
    push2(&t, 100);
    push2(&t, 200);
    push2(&t, 300);
    printf("Popped from stack 1: %d\n", pop1(&t));
    printf("Popped from stack 2: %d\n", pop2(&t));
    push1(&t, 30);
    push1(&t, 40);
    push1(&t, 50);
    push1(&t, 60);
    printf("\n");

    printf("Q15:\n");
    const char *infix = "(A + B) * C - D";
    char prefix[MAX];
    printf("Infix: %s\n", infix);
    infixToPrefix(infix, prefix);
    printf("Prefix: %s\n\n", prefix);

    printf("Q16:\n");
    const char *expr = "- * + 4 3 2 5";
    printf("Prefix: %s\n", expr);
    printf("Result = %d\n\n", evaluatePrefix(expr));

    printf("Q17:\n");
    int price[] = {100, 80, 60, 70, 60, 75, 85};
    int span[7];
    stockSpan(price, 7, span);
    printf("Price: ");
    for (int i = 0; i < 7; i++) printf("%d ", price[i]);
    printf("\nSpan : ");
    for (int i = 0; i < 7; i++) printf("%d ", span[i]);
    printf("\n");

    return 0;
}
