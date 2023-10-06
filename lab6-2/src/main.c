#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

typedef struct BTreeNode {
    int number_of_keys; 
    int* keys;
    struct BTreeNode** children;
} BTreeNode;

typedef struct BTree {
    int t;
    int height;
    BTreeNode* root;
} BTree;

BTreeNode* CreateBTreeNode(BTree* btree, bool is_leaf) {
    BTreeNode* btree_node = (BTreeNode*) malloc (sizeof(BTreeNode));
    btree_node->number_of_keys = 0;
    btree_node->keys = (int*) malloc (sizeof(int) * (2 * btree->t - 1));
    if (is_leaf) {
        btree_node->children = NULL;
    } else {
        btree_node->children = (BTreeNode**) malloc (sizeof(BTreeNode*) * 2 * btree->t);
    }
    return btree_node;
}

bool IsLeaf(BTreeNode* node) {
    return node->children == NULL;
}

bool IsFull(BTreeNode* node, BTree* tree) {
    return node->number_of_keys == 2 * tree->t - 1;
}

void MoveTail(BTreeNode* src, int i, BTreeNode* dst, int j) {
    int move = dst->number_of_keys - j;
    memmove(dst->keys + j, src->keys + i, move * sizeof(int));
    if (!IsLeaf(src)) {
        memmove(dst->children + j, src->children + i, (move + 1) * sizeof(BTreeNode*));
    }

}

void Split(int move_index, BTreeNode* parent_node, BTree* btree) {
    BTreeNode* existing_child = parent_node->children[move_index];
    BTreeNode* new_child = IsLeaf(existing_child) ? CreateBTreeNode(btree, 1) : CreateBTreeNode(btree, 0);
    new_child->number_of_keys = btree->t - 1;
    MoveTail(existing_child, btree->t, new_child, 0);
    existing_child->number_of_keys = btree->t - 1;
    parent_node->number_of_keys += 1;
    MoveTail(parent_node, move_index, parent_node, move_index + 1);
    parent_node->keys[move_index] = existing_child->keys[btree->t - 1];
    parent_node->children[move_index + 1] = new_child;
}

int GetChildIndex(BTreeNode* node, int key) {
    int l = 0;
    int r = node->number_of_keys - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (key <= node->keys[m]) {
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return r + 1;
}

void AddKey(BTreeNode* node, int k) {
    int c = GetChildIndex(node, k);
    node->number_of_keys++;
    MoveTail(node, c, node, c + 1);
    node->keys[c] = k;
}

void InsertNonFull(BTree* btree, BTreeNode* node, int to_add) {
    if (IsLeaf(node)) {
        AddKey(node, to_add);
    } else {
        int child_index = GetChildIndex(node, to_add);
        if (IsFull(node->children[child_index], btree)) {
            Split(child_index, node, btree);
        }
        InsertNonFull(btree, node->children[GetChildIndex(node, to_add)], to_add);
    }
}

void Insert(BTree* btree, int k) {
    if (IsFull(btree->root, btree)) {
        BTreeNode* new_root = CreateBTreeNode(btree, 0);
        new_root->children[0] = btree->root;
        Split(0, new_root, btree);
        btree->root = new_root;
        btree->height++;
    }
    InsertNonFull(btree, btree->root, k);
}

void DestroyBTree(BTreeNode* node) {
    free(node->keys);
    if (IsLeaf(node)) {
        free(node);
        return;
    }
    for (int i = 0; i <= node->number_of_keys; i++) {
        DestroyBTree(node->children[i]);
    }
    free(node->children);
    free(node);
}

int main() {
    int t;
    int number_to_add;
    if (!scanf("%d %d", &t, &number_to_add)) {
        return 0;
    }
    if (number_to_add == 0) {
        printf("0");
        return 0;
    }
    BTree b_tree = {t, 0, NULL};
    BTreeNode* root = CreateBTreeNode(&b_tree, 1);
    b_tree.root = root;
    for (int i = 0; i < number_to_add; i++) {
        int to_add;
        if (!scanf("%d", &to_add)) {
            return 0;
        }
        Insert(&b_tree, to_add);
    }
    printf("%d", b_tree.height + 1);
    DestroyBTree(b_tree.root);
    return 0;
}
