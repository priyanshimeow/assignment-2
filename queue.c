#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int data[MAX];
    int front;
    int size;
} Queue;

void initQueue(Queue *q) {
    q->front = 0;
    q->size = 0;
}

int isEmpty(Queue *q) { return q->size == 0; }

void enqueue(Queue *q, int x) {
    q->data[(q->front + q->size) % MAX] = x;
    q->size++;
}

int dequeue(Queue *q) {
    int x = q->data[q->front];
    q->front = (q->front + 1) % MAX;
    q->size--;
    return x;
}

int peekFront(Queue *q) { return q->data[q->front]; }

void printQueue(const char *label, Queue *q) {
    printf("%s (front -> rear): ", label);
    for (int i = 0; i < q->size; i++) printf("%d ", q->data[(q->front + i) % MAX]);
    printf("\n");
}

/* Q18. Reverse the first k elements of a queue. */
void reverseFirstK(Queue *q, int k) {
    int n = q->size;
    if (k <= 0 || k > n) return;
    int stk[MAX];
    int top = -1;
    for (int i = 0; i < k; i++) stk[++top] = dequeue(q);
    while (top >= 0) enqueue(q, stk[top--]);
    for (int i = 0; i < n - k; i++) enqueue(q, dequeue(q));
}

/* Q19. Interleave the first half of a queue with the second half. */
void interleave(Queue *q) {
    int half = q->size / 2;
    Queue firstHalf;
    initQueue(&firstHalf);
    for (int i = 0; i < half; i++) enqueue(&firstHalf, dequeue(q));
    while (!isEmpty(&firstHalf)) {
        enqueue(q, dequeue(&firstHalf));
        enqueue(q, dequeue(q));
    }
}

/* Q20. Implement a deque using a circular array (insertFront, insertRear, deleteFront, deleteRear). */
#define CAP 5

typedef struct {
    int arr[CAP];
    int front;
    int count;
} Deque;

void initDeque(Deque *d) {
    d->front = 0;
    d->count = 0;
}

void insertFront(Deque *d, int x) {
    if (d->count == CAP) {
        printf("Deque Overflow\n");
        return;
    }
    d->front = (d->front - 1 + CAP) % CAP;
    d->arr[d->front] = x;
    d->count++;
}

void insertRear(Deque *d, int x) {
    if (d->count == CAP) {
        printf("Deque Overflow\n");
        return;
    }
    d->arr[(d->front + d->count) % CAP] = x;
    d->count++;
}

int deleteFront(Deque *d) {
    if (d->count == 0) {
        printf("Deque Underflow\n");
        return -1;
    }
    int x = d->arr[d->front];
    d->front = (d->front + 1) % CAP;
    d->count--;
    return x;
}

int deleteRear(Deque *d) {
    if (d->count == 0) {
        printf("Deque Underflow\n");
        return -1;
    }
    int x = d->arr[(d->front + d->count - 1) % CAP];
    d->count--;
    return x;
}

void printDeque(Deque *d) {
    printf("Deque (front -> rear): ");
    for (int i = 0; i < d->count; i++) printf("%d ", d->arr[(d->front + i) % CAP]);
    printf("\n");
}

/* Q21. Simulate Round Robin CPU scheduling using a queue. */
void roundRobin(int burst[], int n, int quantum) {
    int remaining[MAX], completion[MAX];
    Queue q;
    initQueue(&q);
    for (int i = 0; i < n; i++) {
        remaining[i] = burst[i];
        enqueue(&q, i);
    }

    int time = 0;
    printf("Gantt chart:\n");
    while (!isEmpty(&q)) {
        int p = dequeue(&q);
        int run = remaining[p] < quantum ? remaining[p] : quantum;
        printf("  t=%d to t=%d : P%d\n", time, time + run, p + 1);
        time += run;
        remaining[p] -= run;
        if (remaining[p] > 0) enqueue(&q, p);
        else completion[p] = time;
    }

    printf("\nProcess  Burst  Completion  Turnaround  Waiting\n");
    float totalTat = 0, totalWt = 0;
    for (int i = 0; i < n; i++) {
        int tat = completion[i];
        int wt = tat - burst[i];
        totalTat += tat;
        totalWt += wt;
        printf("P%-7d %-6d %-11d %-11d %d\n", i + 1, burst[i], completion[i], tat, wt);
    }
    printf("Average turnaround time = %.2f\n", totalTat / n);
    printf("Average waiting time    = %.2f\n", totalWt / n);
}

/* Q22. Find the first non-repeating character in a stream of characters using a queue. */
void firstNonRepeating(const char *stream) {
    int freq[256] = {0};
    Queue q;
    initQueue(&q);
    for (int i = 0; stream[i] != '\0'; i++) {
        unsigned char c = stream[i];
        freq[c]++;
        enqueue(&q, c);
        while (!isEmpty(&q) && freq[peekFront(&q)] > 1) dequeue(&q);
        if (isEmpty(&q)) printf("After '%c' : none\n", c);
        else printf("After '%c' : %c\n", c, peekFront(&q));
    }
}

/* Q23. Solve the Josephus problem using a queue. */
int josephus(int n, int k) {
    Queue q;
    initQueue(&q);
    for (int i = 1; i <= n; i++) enqueue(&q, i);
    printf("Elimination order: ");
    while (q.size > 1) {
        for (int i = 0; i < k - 1; i++) enqueue(&q, dequeue(&q));
        printf("%d ", dequeue(&q));
    }
    printf("\n");
    return dequeue(&q);
}

/* Q24. Insert and delete in a priority queue implemented with a linked list. State the complexity of each. */
typedef struct Node {
    int data;
    int priority;
    struct Node *next;
} Node;

void pqInsert(Node **head, int data, int priority) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->data = data;
    n->priority = priority;
    if (*head == NULL || priority < (*head)->priority) {
        n->next = *head;
        *head = n;
        return;
    }
    Node *cur = *head;
    while (cur->next != NULL && cur->next->priority <= priority) cur = cur->next;
    n->next = cur->next;
    cur->next = n;
}

int pqDelete(Node **head, int *priority) {
    if (*head == NULL) {
        printf("Priority queue is empty\n");
        return -1;
    }
    Node *t = *head;
    int data = t->data;
    *priority = t->priority;
    *head = t->next;
    free(t);
    return data;
}

void pqPrint(Node *head) {
    printf("Priority queue (data:priority): ");
    for (Node *c = head; c != NULL; c = c->next) printf("%d:%d ", c->data, c->priority);
    printf("\n");
}

int main(void) {
    printf("Q18:\n");
    Queue q18;
    initQueue(&q18);
    for (int i = 1; i <= 8; i++) enqueue(&q18, i * 10);
    printQueue("Before", &q18);
    reverseFirstK(&q18, 4);
    printQueue("After", &q18);
    printf("\n");

    printf("Q19:\n");
    Queue q19;
    initQueue(&q19);
    for (int i = 1; i <= 10; i++) enqueue(&q19, i);
    printQueue("Before", &q19);
    interleave(&q19);
    printQueue("After", &q19);
    printf("\n");

    printf("Q20:\n");
    Deque d;
    initDeque(&d);
    insertRear(&d, 10);
    insertRear(&d, 20);
    insertFront(&d, 5);
    insertFront(&d, 1);
    printDeque(&d);
    printf("deleteFront: %d\n", deleteFront(&d));
    printf("deleteRear : %d\n", deleteRear(&d));
    printDeque(&d);
    insertRear(&d, 30);
    insertRear(&d, 40);
    insertFront(&d, 0);
    printDeque(&d);
    insertRear(&d, 50);
    printf("\n");

    printf("Q21:\n");
    int burst[] = {5, 3, 8, 6};
    roundRobin(burst, 4, 3);
    printf("\n");

    printf("Q22:\n");
    firstNonRepeating("aabcbd");
    printf("\n");

    printf("Q23:\n");
    int n = 7, k = 3;
    int survivor = josephus(n, k);
    printf("Survivor position = %d\n\n", survivor);

    printf("Q24:\n");
    Node *head = NULL;
    pqInsert(&head, 100, 3);
    pqInsert(&head, 200, 1);
    pqInsert(&head, 300, 2);
    pqInsert(&head, 400, 1);
    pqPrint(head);
    int pr;
    int x = pqDelete(&head, &pr);
    printf("Deleted %d (priority %d)\n", x, pr);
    pqPrint(head);
    printf("Complexity: Insert O(n), Delete O(1)\n");
    while (head != NULL) pqDelete(&head, &pr);

    return 0;
}
