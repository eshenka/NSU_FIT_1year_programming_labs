#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ASCII_size 256
#define BITS_IN_CHAR 8
#define NUMBER_OF_CHILDREN 2

typedef struct TreeNode {
    int frequency;
    unsigned char letter;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct CodesTableNode {
    unsigned long long code;
    int code_len;
} CodesTableNode;

void CreateASCIITable(FILE* in, int* table) {
    while (!feof(in)) {
        unsigned char input = fgetc(in);
        if (feof(in)) {
            return;
        }
        table[input]++;
    }
}

int GetIndex(TreeNode** f_table, int f, int* table_len) {
    int l = 0;
    int r = (*table_len) - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (f <= f_table[m]->frequency) {
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return r + 1;
}

void TableAdd(TreeNode** f_table, int f, int* table_len, TreeNode* node) {
    int move_index = GetIndex(f_table, f, table_len);
    (*table_len)++;
    memmove(f_table + move_index + 1, f_table + move_index, (*table_len - move_index - 1) * sizeof(TreeNode*));
    f_table[move_index] = node;
}

void CreateFrequencyTable(int* ASCII_table, TreeNode** f_table, int* table_len) {
    for (int i = 0; i < ASCII_size; i++) {
        int f = ASCII_table[i];

        if (f <= 0) continue;

        TreeNode* node = (TreeNode*) malloc (sizeof(TreeNode));

        node->left = NULL;
        node->right = NULL;
        node->letter = i;
        node->frequency = f;

        TableAdd(f_table, f, table_len, node);
    }
}

TreeNode* CreateTreeFromFrequencyTable(TreeNode** f_table, int* table_len) {
    while (1) {
        if (*table_len == 1) {
            break;
        }

        TreeNode* parent_node = (TreeNode*) malloc (sizeof(TreeNode));
        TreeNode* left_child = f_table[0];
        TreeNode* right_child = f_table[1];

        parent_node->left = left_child;
        parent_node->right = right_child;
        parent_node->frequency = left_child->frequency + right_child->frequency;

        memmove(f_table, f_table + NUMBER_OF_CHILDREN, (*table_len - NUMBER_OF_CHILDREN) * sizeof(TreeNode*));
        (*table_len) -= NUMBER_OF_CHILDREN;

        TableAdd(f_table, parent_node->frequency, table_len, parent_node);
    }

    return f_table[0];
}

bool IsLeaf(TreeNode* node) {
    return node->left == NULL && node->right == NULL;
}

void CreateCodesTable(TreeNode* node, unsigned long long code, int code_len, CodesTableNode* codes_table) {
    if (IsLeaf(node)) {
        codes_table[node->letter].code = code;
        codes_table[node->letter].code_len = code_len == 0 ? 1 : code_len;
    } else {
        CreateCodesTable(node->left, code << 1, code_len + 1, codes_table);
        CreateCodesTable(node->right, (code << 1) | 1, code_len + 1, codes_table);
    }
}

void AddBit(FILE* out, bool bit, int* cur_number_of_bits, unsigned char* cur_status) {
    if ((*cur_number_of_bits) == BITS_IN_CHAR) {
        fwrite(cur_status, 1, 1, out);
        *cur_number_of_bits = 0;
        *cur_status = 0;
    }
    if (bit) {
        (*cur_status) = ((*cur_status) << 1) | 1;
    } else {
        (*cur_status) = (*cur_status) << 1;
    }
    (*cur_number_of_bits)++;
}

void WriteLetter(FILE* out, unsigned char letter, int* cur_number_of_bits, unsigned char* cur_status) {
    for (int i = BITS_IN_CHAR - 1; i >= 0; i--) {
        if ((letter >> i) & 1) {
            AddBit(out, 1, cur_number_of_bits, cur_status);
        } else {
            AddBit(out, 0, cur_number_of_bits, cur_status);
        }
    }
}

void WriteEncodedTree(FILE* out, TreeNode* node, int* cur_number_of_bits, unsigned char* cur_status) {
    if (IsLeaf(node)) {
        AddBit(out, 1, cur_number_of_bits, cur_status);
        WriteLetter(out, node->letter, cur_number_of_bits, cur_status);
    } else {
        AddBit(out, 0, cur_number_of_bits, cur_status);
        WriteEncodedTree(out, node->left, cur_number_of_bits, cur_status);
        WriteEncodedTree(out, node->right, cur_number_of_bits, cur_status);
    }
}

void WriteEncodedText(FILE* in, FILE* out, CodesTableNode* codes_table, int* cur_number_of_bits, unsigned char* cur_status) {
    fseek(in, 1, SEEK_SET);
    while (1) {        
        unsigned char input = fgetc(in);

        if (feof(in)) {
            break;
        }

        unsigned long long code = codes_table[input].code;
        int code_len = codes_table[input].code_len;

        for (int i = code_len - 1; i >= 0; i--) {
            if ((code >> i) & 1) {
                AddBit(out, 1, cur_number_of_bits, cur_status);
            } else {
                AddBit(out, 0, cur_number_of_bits, cur_status);
            }
        }
    }
}

void AddRest(FILE* out, unsigned char* cur_status, int cur_number_of_bits) {
    if (cur_number_of_bits != 0) {
        (*cur_status) = (*cur_status) << (BITS_IN_CHAR - cur_number_of_bits);
        fwrite(cur_status, 1, 1, out);
    }
}

void FreeTree(TreeNode* node) {
    if (IsLeaf(node)) {
        free(node);
        return;
    }
    FreeTree(node->left);
    FreeTree(node->right);
    free(node);
}

void Encode(FILE* in, FILE* out) {
    int ASCII_table[ASCII_size] = { 0 };
    CreateASCIITable(in, ASCII_table);

    TreeNode* frequency_table[ASCII_size];
    int frequency_table_len = 0;
    CreateFrequencyTable(ASCII_table, frequency_table, &frequency_table_len);

    TreeNode* root = CreateTreeFromFrequencyTable(frequency_table, &frequency_table_len);

    unsigned char empty_byte_for_remainder = 0;
    fwrite(&empty_byte_for_remainder, 1, 1, out);

    CodesTableNode codes_table[ASCII_size];
    CreateCodesTable(root, 0, 0, codes_table);

    int cur_number_of_bits = 0;
    unsigned char cur_status = 0;

    WriteEncodedTree(out, root, &cur_number_of_bits, &cur_status);
    AddRest(out, &cur_status, cur_number_of_bits);
    cur_number_of_bits = 0;

    WriteEncodedText(in, out, codes_table, &cur_number_of_bits, &cur_status);
    AddRest(out, &cur_status, cur_number_of_bits);

    fseek(out, 0, SEEK_SET);
    unsigned char remainder = (unsigned char)cur_number_of_bits;
    fwrite(&remainder, 1, 1, out);

    FreeTree(root);
}

bool ReadBit(unsigned char cur_status, int cur_number_of_bits) {
    return ((cur_status) >> (BITS_IN_CHAR - (cur_number_of_bits))) & 1;
}

unsigned char ReadLetter(FILE* in, int* cur_number_of_bits, unsigned char* cur_status) {
    unsigned char letter = 0;
    letter = letter | ((*cur_status) << (*cur_number_of_bits));

    *cur_status = fgetc(in);

    (*cur_number_of_bits) = BITS_IN_CHAR - (*cur_number_of_bits);
    letter = letter | ((*cur_status) >> (*cur_number_of_bits));
    (*cur_number_of_bits) = BITS_IN_CHAR - (*cur_number_of_bits);

    return letter;
}

TreeNode* GetTreeFromFile(FILE* in, int* cur_number_of_bits, unsigned char* cur_status) {
    (*cur_number_of_bits)++;

    if ((*cur_number_of_bits) >= BITS_IN_CHAR + 1) {
        *cur_status = fgetc(in);
        (*cur_number_of_bits) = 1;
    }

    if (ReadBit(*cur_status, *cur_number_of_bits)) {
        unsigned char letter = ReadLetter(in, cur_number_of_bits, cur_status);
        TreeNode* new_node = (TreeNode*) malloc (sizeof(TreeNode));
        *new_node = (TreeNode) {0, letter, NULL, NULL};

        return new_node;
    } else {
        TreeNode* node = (TreeNode*) malloc (sizeof(TreeNode));

        node->left = GetTreeFromFile(in, cur_number_of_bits, cur_status);
        node->right = GetTreeFromFile(in, cur_number_of_bits, cur_status);

        return node;
    }
}

void PrintLetter(FILE* out, unsigned char letter) {
    fwrite(&letter, 1, 1, out);
}

void PrintText(FILE* in, FILE* out, TreeNode* root, unsigned char rem) {
    TreeNode* node = root;
    unsigned char cur_status;
    cur_status = fgetc(in);
    while (1)  {
        unsigned char next_status = fgetc(in);
        for (int i = 1; i <= BITS_IN_CHAR; i++) {
            if (!IsLeaf(node)) {
                node = ReadBit(cur_status, i) ? node->right : node->left;
            }

            if (IsLeaf(node)) {
                PrintLetter(out, node->letter);
                node = root;
            }

            if (feof(in) && i >= rem) {
                return;
            }
        }
        cur_status = next_status;
    }
}

void Decode(FILE* in, FILE* out) {
    unsigned char remainder;
    remainder = fgetc(in);

    int cur_number_of_bits = 0;
    unsigned char cur_status = fgetc(in);

    TreeNode* root = GetTreeFromFile(in, &cur_number_of_bits, &cur_status);
    
    PrintText(in, out, root, remainder);

    FreeTree(root);
}

int main() {
    FILE* in = fopen("in.txt", "rb");
    FILE* out = fopen("out.txt", "wb");

    unsigned char to_do = fgetc(in);

    fgetc(in);
    if (feof(in)) {
        fclose(in);
        fclose(out);
        return 0;
    }

    fseek(in, 1, SEEK_SET);

    if (to_do == 'c') {
        Encode(in, out);
    } else {
        Decode(in, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}
