#include <stdio.h>
#include <math.h>
#include <string.h>
#define kMaxArraySize 17
#define kBufferSize 1024*256

int PowThree[16];

void CreatePowThreeArray(int* PowThree) {
    PowThree[0] = 1;
    for (int i = 1; i < 16; i++) {
        PowThree[i] = PowThree[i - 1] * 3;
    }   
}

int GetSampleHash(FILE* fp, unsigned char* sample, int* sample_length) {
    int hash = 0, check_scanf = 1, i = 0;
    while (!feof(fp) && check_scanf) {
        check_scanf = fscanf(fp, "%c", &sample[i]);
        if (sample[i] == '\n') { 
            break;
        }
        *sample_length += 1;
        hash += (sample[i] % 3) * PowThree[i];
        i++;
    }
    return hash;
}

int GetStrHash(FILE* fp, unsigned char* str, int sample_length) {
    int hash = 0, check_scanf = 1;
    for (int i = 0; i < sample_length; i++) {
        if (!feof(fp) && check_scanf) {
            check_scanf = fscanf(fp, "%c", &str[i]);
        }
        if (feof(fp)) {
            return hash;
        }
        hash += (str[i] % 3) * PowThree[i];
    }
    return hash;
}

int UpdateHash(unsigned char* a, int hash, int i, int sample_length) {
    i--;
    hash -= a[i] % 3;
    hash /= 3;
    hash += (a[i + sample_length] % 3) * PowThree[sample_length - 1];
    return hash;
}

void Rewrite(FILE* fp, unsigned char* str, int sample_length, int* pointer, int* space, int* str_length, int* str_hash) {
    int shift = sample_length - 1;
    *str_hash -= str[*pointer] % 3;
    *str_hash /= 3;
    for (int i = 0; i < shift; i++, *pointer+=1) {
        str[i] = str[*pointer];
    }
    *space += *str_length - shift;
    *str_length = (int)fread(str + shift, sizeof(unsigned char), *str_length - shift, fp);
    *str_length += shift;
    *str_hash += (str[shift] % 3) * PowThree[sample_length - 1];
    *pointer = 1;
}

void PrintComparison(unsigned char* str, unsigned char* sample, int sample_length, int space, int pointer) {
    for (int i = 0; i < sample_length; i++, pointer++) {
        printf("%d ", space + pointer + 1);
        if (sample[i] != str[pointer]) {
            break;
        }
    }

}

void CompareHashes(FILE* fp, unsigned char* str, unsigned char* sample, int str_length, int sample_length, int sample_hash, int str_hash) {
    int pointer = 0, space = 0;
    if (sample_hash == str_hash) {
        PrintComparison(str, sample, sample_length, space, pointer);
    }
    pointer++;
    while (pointer + sample_length - 1 < str_length) {
        str_hash = UpdateHash(str, str_hash, pointer, sample_length);
        if (sample_hash == str_hash) {
            PrintComparison(str, sample, sample_length, space, pointer);
        }
        pointer++;
        if (pointer + sample_length - 1 >= str_length && !feof(fp)) {
            Rewrite(fp, str, sample_length, &pointer, &space, &str_length, &str_hash);
        }
    }
}

int main() {
    CreatePowThreeArray(PowThree);
    FILE* fp = fopen("in.txt", "r");
    unsigned char sample[kMaxArraySize], str[kBufferSize];
    int sample_length = 0;
    int str_length = kBufferSize;
    int sample_hash = GetSampleHash(fp, sample, &sample_length);
    printf("%d ", sample_hash);
    int str_hash = GetStrHash(fp, str, sample_length);
    if (str_hash == -1) {
        fclose(fp);
        return 0;
    }
    if (!feof(fp)) {
        str_length = (int)fread(str + sample_length, sizeof(char), str_length - sample_length, fp);
        str_length += sample_length;
        CompareHashes(fp, str, sample, str_length, sample_length, sample_hash, str_hash);
    }
    fclose(fp);
    return 0;
}
