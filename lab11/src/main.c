#include <stdio.h>
#include <malloc.h>

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

void FillProfitTable(int number, int capacity, int* table[], const int weighs[], int prices[]) {
    for (int row = 0; row <= number; row++) {
        for (int column = 0; column <= capacity; column++) {
            if (row == 0 || column == 0) {
                table[row][column] = 0;
            } else if (weighs[row] <= column) {
                table[row][column] = max(prices[row] + table[row - 1][column - weighs[row]], table[row - 1][column]);
            } else {
                table[row][column] = table[row-1][column];
            }
        }
    }
}

void GetSumAndListOfItems(int number_of_items, int capacity, const int weighs[], int prices[], int items[]) {
    int** profit_table = (int**) malloc(sizeof(int*) * (number_of_items + 1));
    for (int i = 0; i <= number_of_items; i++) {
        profit_table[i] = (int*) malloc(sizeof(int) * (capacity + 1));
    }
    FillProfitTable(number_of_items, capacity, profit_table, weighs, prices);
    printf("%d\n", profit_table[number_of_items][capacity]);
    int column = capacity;
    int items_taken = 1;
    for (int row = number_of_items; row > 0; row--) {
        if (profit_table[row][column] != profit_table[row-1][column]) {
            items[items_taken] = row;
            items_taken++;
            column -= weighs[row];
        }
    }
    // Авторский код (с) Липаткин
    items[0] = items_taken;
    for (int i = 0; i <= number_of_items; i++) {
        free(profit_table[i]);
    }
    free(profit_table);
}


int main() {
    int number_of_items;
    int capacity;
    if (!scanf("%d %d", &number_of_items, &capacity)) {
        printf("bad input");
        return 0;
    }
    int* weighs = (int*) calloc(number_of_items + 1, sizeof(int));
    int* prices = (int*) calloc(number_of_items + 1, sizeof(int));
    for(int i = 1; i <= number_of_items; i++) {
        if (!scanf("%d %d", &weighs[i], &prices[i])) {
            printf("bad input");
            return 0;
        }
    }
    int* items = (int*) malloc(sizeof(int) * (number_of_items + 1));
    GetSumAndListOfItems(number_of_items, capacity, weighs, prices, items);
    for (int i = items[0]-1; i > 0; i--) {
        printf("%d %d\n", weighs[items[i]], prices[items[i]]);
    }
    free(weighs);
    free(prices);
    free(items);
    return 0;
}
