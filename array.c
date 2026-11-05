#include <stdio.h>
#include <string.h>

#define MAX 100
#define TABLE_SIZE 1009

void printArray(const char *label, int a[], int n) {
    printf("%s: ", label);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

/* Q1. Insert an element into a sorted array so that it stays sorted. State the complexity. */
void insertSorted(int a[], int *n, int x) {
    int i = *n - 1;
    while (i >= 0 && a[i] > x) {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = x;
    (*n)++;
}

/* Q2. Remove duplicates from a sorted array in-place. */
int removeDuplicates(int a[], int n) {
    if (n == 0) return 0;
    int j = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] != a[j]) {
            j++;
            a[j] = a[i];
        }
    }
    return j + 1;
}

/* Q3. Delete all occurrences of a given value from an array in-place. */
int deleteValue(int a[], int n, int val) {
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] != val) a[k++] = a[i];
    }
    return k;
}

/* Q4. Merge two sorted arrays into one sorted array. */
int mergeSorted(int a[], int m, int b[], int n, int res[]) {
    int i = 0, j = 0, k = 0;
    while (i < m && j < n) {
        if (a[i] <= b[j]) res[k++] = a[i++];
        else res[k++] = b[j++];
    }
    while (i < m) res[k++] = a[i++];
    while (j < n) res[k++] = b[j++];
    return k;
}

/* Q5. Move all zeroes to the end of an array while keeping the order of other elements. */
void moveZeroes(int a[], int n) {
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] != 0) a[k++] = a[i];
    }
    while (k < n) a[k++] = 0;
}

/* Q6. Find the largest and second largest elements in an array in a single traversal. */
int findTwoLargest(int a[], int n, int *largest, int *second) {
    if (n < 2) return 0;
    int first = a[0] > a[1] ? a[0] : a[1];
    int sec = a[0] > a[1] ? a[1] : a[0];
    for (int i = 2; i < n; i++) {
        if (a[i] > first) {
            sec = first;
            first = a[i];
        } else if (a[i] > sec && a[i] != first) {
            sec = a[i];
        }
    }
    *largest = first;
    *second = sec;
    return 1;
}

/* Q7. Solve the Two Sum problem using brute force and using hashing. Compare time and space complexity. */
int twoSumBrute(int a[], int n, int target, int *p, int *q) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] + a[j] == target) {
                *p = i;
                *q = j;
                return 1;
            }
    return 0;
}

typedef struct {
    int key;
    int idx;
    int used;
} Entry;

int hashFn(int key) {
    return ((key % TABLE_SIZE) + TABLE_SIZE) % TABLE_SIZE;
}

int twoSumHash(int a[], int n, int target, int *p, int *q) {
    Entry table[TABLE_SIZE];
    memset(table, 0, sizeof(table));

    for (int i = 0; i < n; i++) {
        int need = target - a[i];

        int h = hashFn(need);
        while (table[h].used) {
            if (table[h].key == need) {
                *p = table[h].idx;
                *q = i;
                return 1;
            }
            h = (h + 1) % TABLE_SIZE;
        }

        h = hashFn(a[i]);
        while (table[h].used) h = (h + 1) % TABLE_SIZE;
        table[h].key = a[i];
        table[h].idx = i;
        table[h].used = 1;
    }
    return 0;
}

/* Q8. Find the maximum subarray sum using Kadane's Algorithm and dry run it on an example. */
int kadane(int a[], int n) {
    int currentSum = a[0];
    int maxSum = a[0];

    printf("  i | a[i] | currentSum | maxSum\n");
    printf("  0 | %4d | %10d | %6d\n", a[0], currentSum, maxSum);

    for (int i = 1; i < n; i++) {
        currentSum = (a[i] > currentSum + a[i]) ? a[i] : currentSum + a[i];
        if (currentSum > maxSum) maxSum = currentSum;
        printf("  %d | %4d | %10d | %6d\n", i, a[i], currentSum, maxSum);
    }
    return maxSum;
}

int main(void) {
    printf("Q1. Insert into sorted array\n");
    int a1[MAX] = {10, 20, 30, 40, 50};
    int n1 = 5;
    printArray("Before", a1, n1);
    insertSorted(a1, &n1, 35);
    printArray("After inserting 35", a1, n1);
    printf("Complexity: Time O(n), Space O(1)\n\n");

    printf("Q2. Remove duplicates from sorted array\n");
    int a2[MAX] = {1, 1, 2, 2, 2, 3, 4, 4, 5};
    int n2 = 9;
    printArray("Before", a2, n2);
    n2 = removeDuplicates(a2, n2);
    printArray("After", a2, n2);
    printf("\n");

    printf("Q3. Delete all occurrences of a value\n");
    int a3[MAX] = {3, 2, 3, 4, 3, 5, 3};
    int n3 = 7;
    printArray("Before", a3, n3);
    n3 = deleteValue(a3, n3, 3);
    printArray("After deleting 3", a3, n3);
    printf("\n");

    printf("Q4. Merge two sorted arrays\n");
    int x[] = {1, 3, 5, 7};
    int y[] = {2, 4, 6, 8, 10};
    int merged[MAX];
    int nm = mergeSorted(x, 4, y, 5, merged);
    printArray("Merged", merged, nm);
    printf("\n");

    printf("Q5. Move zeroes to the end\n");
    int a5[] = {0, 1, 0, 3, 12, 0, 5};
    int n5 = 7;
    printArray("Before", a5, n5);
    moveZeroes(a5, n5);
    printArray("After", a5, n5);
    printf("\n");

    printf("Q6. Largest and second largest\n");
    int a6[] = {12, 35, 1, 10, 34, 1};
    int large, second;
    if (findTwoLargest(a6, 6, &large, &second))
        printf("Largest = %d, Second largest = %d\n\n", large, second);

    printf("Q7. Two Sum (target = 9)\n");
    int a7[] = {2, 7, 11, 15};
    int p, q;
    if (twoSumBrute(a7, 4, 9, &p, &q))
        printf("Brute force: indices %d and %d\n", p, q);
    if (twoSumHash(a7, 4, 9, &p, &q))
        printf("Hashing    : indices %d and %d\n", p, q);
    printf("Comparison:\n");
    printf("  Brute force -> Time O(n^2), Space O(1)\n");
    printf("  Hashing     -> Time O(n) average, Space O(n)\n\n");

    printf("Q8. Kadane's Algorithm (dry run)\n");
    int a8[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    printf("Array: -2 1 -3 4 -1 2 1 -5 4\n");
    int best = kadane(a8, 9);
    printf("Maximum subarray sum = %d\n", best);

    return 0;
}
