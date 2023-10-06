#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ARRAY_SIZE 10

typedef enum Errors {
    NO_ERRORS,
    BAD_INPUT,
    OVERFLOW,
} Errors;

void PrintError(Errors error) {
    switch (error) {
    case BAD_INPUT:
        printf("bad input");
        exit(0);
    case OVERFLOW:
        printf("overflow");
        exit(0);
    default:
        return;
    }
}

void CheckError(Errors error) {
    if (error != NO_ERRORS) {
        PrintError(error);
    }
}

void GetNumber(int *number_of_elements, Errors *error) {
    if (scanf("%d", number_of_elements) != 1) {
        *error = BAD_INPUT;
        return;
    }
    if (*number_of_elements < 1 || *number_of_elements > 10) {
        *error = BAD_INPUT;
        return;
    }
}

void GetArrayElements(int number_of_elements, long long *array, Errors *error) {
    for (int i = 0; i < number_of_elements; i++) {
        long long element;
        if (scanf("%lld", &element) < 1)
        {
            *error = BAD_INPUT;
            return;
        }
        if (element > INT_MAX || element < INT_MIN)
        {
            *error = BAD_INPUT;
            return;
        }
        array[i] = element;
    }
}

long long GetSum(int number_of_elements, long long *array, Errors *error) {
    long long sum = 0;
    for (int i = 0; i < number_of_elements; i++)
    {
        sum += array[i];
    }
    if (sum > INT_MAX || sum < INT_MIN)
    {
        *error = OVERFLOW;
    }
    return sum;
}

int main() {
    Errors error = NO_ERRORS;

    int number_of_elements;
    GetNumber(&number_of_elements, &error);
    CheckError(error);

    long long array[ARRAY_SIZE] = {0};
    GetArrayElements(number_of_elements, array, &error);
    CheckError(error);

    long long sum = GetSum(number_of_elements, array, &error);
    CheckError(error);
    printf("%lld", sum);
    exit(EXIT_SUCCESS);
}
