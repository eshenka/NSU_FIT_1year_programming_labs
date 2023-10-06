#include <stdio.h>
#define kMaxSize 10

const char numbers[kMaxSize] = "0123456789";

void PrintArray(int *a, int n) {
    for (int j = 0; j < n; j++) {
        printf("%d", a[j]);
    }
    printf("\n");
}

void Swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int GetMaxInd(int *a, int n) {
    for (int i = n - 2; i >= 0; i--) {
        if (a[i] < a[i + 1]) {
            return i;
        }
    }
    return -1;
}

int GetSwapInd(int *a, int n, int to_swap) {
    for (int j = n - 1; j >= 0; j--) {
        if (a[j] > to_swap) {
            return j;
        }
    }
    return -1;
}

void Reverse(int *a, int n, int left) {
    int med = (n + left) / 2;
    for (int i = 0; left < med; left++, i++) {
        Swap(&a[left], &a[n - 1 - i]);
    }
}

int Permutation(int *a, int n) {
    int ind1 = GetMaxInd(a, n);
    if (ind1 == -1) {
        return 0;
    }
    int elem = a[ind1];
    int ind2 = GetSwapInd(a, n, elem);
    if (ind2 == -1) {
        return 0;
    }
    Swap(&a[ind1], &a[ind2]);
    Reverse(a, n, ind1 + 1);
    return 1;
}

int main() {
    int to_permutate[kMaxSize] = { 0 };
    int numbers_encountered[kMaxSize] = { 0 };
    int length = 0;
    char received_number;
    while (scanf("%c", &received_number)) {
        if (received_number == '\n') {
            break;
        }
        int invalid_char = 0;
        for (int i = 0; i < kMaxSize; i++) {
            if (received_number != numbers[i]) {
                invalid_char++;
            }
        }
        if (invalid_char == kMaxSize) {
            printf("bad input");
            return 0;
        }
        numbers_encountered[received_number - '0'] += 1;
        if (numbers_encountered[received_number - '0'] > 1) {
            printf("bad input");
            return 0;
        }
        to_permutate[length] = received_number - '0';
        length++;
        if (length > kMaxSize) {
            printf("bad input");
            return 0;
        }
    }
    int number_of_permutations = 0;
    if (!scanf("%d", &number_of_permutations)) {
        return 0;
    }
    for (int i = 0; i < number_of_permutations; i++) {
        if (!Permutation(to_permutate, length)) {
            return 0;
        }
        else {
            PrintArray(to_permutate, length);
        }
    }
    return 0;
}
