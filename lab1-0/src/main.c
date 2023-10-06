#include <stdio.h>
#define kMaxArraySize 17
#define kBufferSize 1024


/*
function returns:
positive number, if everything is alright;
-1, if fscanf was not successful;
-2, if sample length is longer than it has to be
*/
int ReadSampleAndGetLength(unsigned char* sample) {
    int i = -1;
    int sample_length = 0;
    do {
        i++;
        if (!fscanf(stdin, "%c", &sample[i])) {
            printf("bad input");
            return -1;
        }
        sample_length++;
    } while (!feof(stdin) && sample[i] != '\n');
    if (sample_length > kMaxArraySize) {
        return -2;
    }
    return sample_length - 1;
}

void CreateShiftTable(int* shift_table, unsigned char* sample, int sample_length) {
    int shift = 1;
    for (int i = sample_length - 2; i >= 0; i--) {
        shift_table[i] = shift;
        shift++;
        for (int j = i + 1; j < sample_length - 1; j++) {
            if (sample[i] == sample[j]) {
                shift_table[i] = shift_table[j];
            }
        }
        if (sample[i] == sample[sample_length - 1]) {
            shift_table[sample_length - 1] = shift_table[i];
        }
        else {
            shift_table[sample_length - 1] = sample_length;
        }
    }
}

int GetShift(unsigned char* sample, int sample_length, int* shift_table, char c) {
    for (int i = 0; i < sample_length; i++) {
        if (sample[i] == c) {
            return shift_table[i];
        }
    }
    return sample_length;
}

void PrintComparison(unsigned char* str,
                    unsigned char* sample,
                    int sample_length,
                    int pointer, 
                    int space) {
    int str_index = pointer;
    int sample_index = sample_length - 1;
    while (str_index >= 0 && sample_index >= 0) {
        fprintf(stdout, "%d ", str_index + 1 + space);
        if (str[str_index] == sample[sample_index]) {
            str_index--;
            sample_index--;
        }
        else {
            break;
        }
    }
}

void Rewrite(unsigned char* str, int* str_length,
            unsigned char* sample, int sample_length,
            int* space,
            int* shift_table,
            int* pointer) {
    int buffer_shift = sample_length - GetShift(sample, sample_length, shift_table, str[*pointer]);
    for (int i = 0; i < buffer_shift; i++) {
        str[i] = str[*pointer + i + 1 - buffer_shift];
    }
    *space += *str_length - buffer_shift;
    *str_length = (int)fread(str + buffer_shift, sizeof(char), *str_length - buffer_shift, stdin);
    *str_length += buffer_shift;
    *pointer = sample_length - 1;
}

void Compare(unsigned char* sample, int sample_length, int* shift_table) {
    unsigned char str[kBufferSize];
    int str_length = (int)fread(str, sizeof(char), kBufferSize, stdin);
    int pointer = sample_length - 1;
    int space = 0;
    while (pointer < str_length) {
        PrintComparison(str, sample, sample_length, pointer, space);
        int shift = GetShift(sample, sample_length, shift_table, str[pointer]);
        if (pointer + shift >= str_length) {
            Rewrite(str, &str_length, sample, sample_length, &space, shift_table, &pointer);
        }
        else {
            pointer += shift;
        }
    }
}

int main() {
    unsigned char sample[kMaxArraySize];
    int shift_table[kMaxArraySize];
    int sample_length = ReadSampleAndGetLength(sample);
    if (sample_length <= 0) {
        return 0;
    }
    CreateShiftTable(shift_table, sample, sample_length);
    Compare(sample, sample_length, shift_table);
    return 0;
}
