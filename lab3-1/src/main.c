#include <stdio.h>
#include <malloc.h>

void Swap(int a[], int i, int j) {
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

int Partition(int a[], long left, long right) {
    int pivot = a[(int)((left + right) / 2)];
    int low = (int)left, high = (int)right;
    while (left < right) {
        while (a[low] < pivot) {
            low++;
        }
        while (a[high] > pivot) {
            high--;
        }
        if (low >= high) {
            return high;
        }
        Swap(a, low, high);
        low++;
        high--;
    }
    return high;
}

void QuickSort(int a[], int left, int right) {
    if (left < right) {
        int pivot = Partition(a, left, right);
        QuickSort(a, left, pivot);
        QuickSort(a, pivot + 1, right);
    }
}


int main() {
    int n = 0;
    if (!scanf("%d", &n)) {
        return 0;
    }
    int *to_sort = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (!scanf("%d", &to_sort[i])) {
            return 0;
        }
    }
    QuickSort(to_sort, 0, n - 1);
    for (int i = 0; i < n; i++) {
        printf("%d ", to_sort[i]);
    }
    free(to_sort);
    return 0;
}
