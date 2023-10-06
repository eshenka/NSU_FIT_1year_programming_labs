#include <stdio.h>
#include <malloc.h>

void Swap(int i, int j, int a[], int n)
{
    if (i >= n || j >= n || i < 0 || j < 0)
    {
        return;
    }
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void Sifting(int a[], int n, int i)
{
    int maxind1 = i;
    while (2 * i + 2 < n)
    {
        if (a[i] >= a[2 * i + 1] && a[i] >= a[2 * i + 2])
        {
            break;
        }
        else if (a[2 * i + 1] >= a[2 * i + 2])
        {
            maxind1 = 2 * i + 1;
        }
        else
        {
            maxind1 = 2 * i + 2;
        }
        Swap(i, maxind1, a, n);
        i = maxind1;
    }
    int maxind2 = i;
    while (2 * i + 1 < n)
    {
        if (a[i] <= a[2 * i + 1])
        {
            maxind2 = 2 * i + 1;
        }
        else
        {
            break;
        }
        Swap(i, maxind2, a, n);
        i = maxind2;
    }
}

void HeapSort(int a[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        Sifting(a, n, i);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        Swap(0, i, a, n);
        Sifting(a, i, 0);
    }
}

int main()
{
    int n = 0;
    if (!scanf("%d", &n))
    {
        return 0;
    }
    int *to_sort = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        if (!scanf("%d", &to_sort[i]))
        {
            return 0;
        }
    }
    HeapSort(to_sort, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", to_sort[i]);
    }
    free(to_sort);
    return 0;
}
