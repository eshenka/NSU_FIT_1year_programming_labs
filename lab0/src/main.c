#include <stdio.h>
#include <math.h>
#include <string.h>
#define kMaxArraySize 50

const int min_notation = 2;
const int max_notation = 16;
const char letters[] = "abcdef";
const char capital_letters[] = "ABCDEF";
const char numbers[] = "0123456789abcdef";
size_t dot_index = 0;
int contains_dot = 0;

int IsBadNotation(int n1, int n2) {
    if (n1 < min_notation || n1 > max_notation || n2 < min_notation || n2 > max_notation) {
        return 1;
    }
    return 0;
}

void ReplaceCapitals(char a[], int n1) {
    if (n1 > 9) {
        for (size_t i = 0; i < strlen(a); i++) {
            for (size_t j = 0; j < strlen(letters); j++) {
                if (a[i] == capital_letters[j]) {
                    a[i] = letters[j];
                }
            }
        }
    }
}

int CheckIsBadSymbol(char a[], int n1) {
    for (size_t i = 0; i < strlen(a); i++) {
        int valid_symbol_counter = 0;
        for (int j = 0; j < n1; j++) {
            if (a[i] != numbers[j] && a[i] != '.') {
                valid_symbol_counter++;
            }
        }
        if (valid_symbol_counter == n1) {
            return 1;
        }
        if ((i == strlen(a) - 1 && a[i] == '.') || a[0] == '.') {
            return 1;
        }
    }
    return 0;
}

int IsBadDotIndex(char a[]) {
    for (size_t i = 0; i < strlen(a); i++) {
        if (a[i] == '.') {
            if (contains_dot) {
                return 1;
            }
            contains_dot = 1;
            dot_index = i;
            continue;
        }
    }
    return 0;
}

double DecimalRepresentation(char *a, double decimal, int notation, int degree, int part) {
    for (size_t i = 0; i < strlen(a); i++) {
        int numb = 10;
        for (size_t j = 0; j < 6; j++) {
            numb = 10;
            if (a[i] == letters[j]) {
                numb += (int)j;
                break;
            }
            else {
                numb = a[i] - '0';
            }
        }
        if (part == 1) {
            decimal += numb * pow(notation, degree);
            degree--;
        }
        else {
            decimal += (double)numb / pow(notation, degree);
            degree++;
        }
    }
    return decimal;
}

void NewNotationInt(char *n2_integer, long long decimal_int, int notation2) {
    unsigned long long i = 0;
    unsigned long long remainder = 0;
    if (decimal_int == 0) {
        n2_integer[0] = '0';
    }
    while (decimal_int != 0) {
        if (decimal_int % notation2 > 9) {
            remainder = (unsigned long long) decimal_int % notation2;
            n2_integer[i] = letters[remainder - 10];
        }
        else {
            n2_integer[i] = (decimal_int % notation2) + '0';
        }
        i++;
        decimal_int /= notation2;
    }
}

void NewNotationFrac(char *n2_fractional, double decimal_frac, int notation2) {
    int cnt = 12; //цифр после запятой
    unsigned long long j = 0;
    unsigned long long cc = 0;
    while (cnt != 0 && (decimal_frac * notation2 < notation2)) {
        decimal_frac *= notation2;
        if (decimal_frac > 9) {
            cc = (unsigned long long)decimal_frac;
            n2_fractional[j] = letters[cc - 10];
        }
        else {
            n2_fractional[j] = ((int)decimal_frac) + '0';
        }
        j++;
        cnt--;
        decimal_frac -= (int)decimal_frac;
    }
}

void ReverseN2Integer(char *n2_integer) {
    unsigned long long length = strlen(n2_integer);
    if (length > 1) {
        for (unsigned long long i2 = 0, j2 = length - 1; i2 < j2; i2++, j2--) {
            unsigned long long c2 = n2_integer[i2];
            n2_integer[i2] = n2_integer[j2];
            n2_integer[j2] = c2;
        }
    }
}

int main() {
    int notation1, notation2;
    if (!scanf("%d %d", &notation1, &notation2)) { //считываем основания сс
        return 0;
    }
    char num[kMaxArraySize];
    if (!scanf("%13s", num)) { //считываем число
        return 0;
    }
    if (IsBadNotation(notation1, notation2)) {
        printf("bad input");
        return 0;
    }
    ReplaceCapitals(num, notation1);
    if (CheckIsBadSymbol(num, notation1)) {
        printf("bad input");
        return 0;
    }
    if (IsBadDotIndex(num)) {
        printf("bad input");
        return 0;
    }
    size_t lenn = strlen(num) - dot_index;
    if (contains_dot == 0) {
        dot_index = strlen(num);
    }
    char integer[kMaxArraySize]; //целая часть числа
    char fractional[] = "000000000000"; //дробная часть числа
    strncpy(integer, &num[0], dot_index);
    integer[dot_index] = '\0';
    if (contains_dot) {
        strncpy(fractional, &num[dot_index + 1], lenn + 1);
    }
    double decimal_int_d = 0; //десятичное представление целой части числа
    double decimal_frac = 0; //десятичное представление дробной части числа
    int degree_1 = (int)(strlen(integer)) - 1;
    int degree_2 = 1;
    decimal_int_d = DecimalRepresentation(integer, decimal_int_d, notation1, degree_1, 1);
    decimal_frac = DecimalRepresentation(fractional, decimal_frac, notation1, degree_2, 2);
    long long decimal_int = (long long)decimal_int_d;
    char n2_integer[kMaxArraySize] = { 0 }; //целая часть числа в новой сс
    char n2_fractional[kMaxArraySize] = { 0 }; //дробная часть числа в новой сс
    NewNotationInt(n2_integer, decimal_int, notation2);
    NewNotationFrac(n2_fractional, decimal_frac, notation2);
    ReverseN2Integer(n2_integer);
    int check_zero = 1;
    for (unsigned long long i = 0; i < strlen(fractional); i++) {
        if (fractional[i] != '0') {
            check_zero = 0;
        }
    }
    if (contains_dot == 0) {
        printf("%s", n2_integer);
        return 0;
    }
    else if (check_zero) {
        printf("%s", n2_integer);
        return 0;
    }
    printf("%s", n2_integer);
    printf(".");
    printf("%s", n2_fractional);
    return 0;
}
