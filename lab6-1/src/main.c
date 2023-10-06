#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define RED 0
#define BLACK 1
#define LEFT 0
#define RIGHT 1
#define NUMBER_OF_CHILDREN 2

typedef struct RBTreeNode {
    int value;
    int color;
    struct RBTreeNode* children[NUMBER_OF_CHILDREN];
} RBTreeNode;

bool IsRed(RBTreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->color == RED) {
        return 1;
    } else {
        return 0;
    }
}

void SwapColors(RBTreeNode *root) {
    root->color = !root->color;
    root->children[0]->color = !root->children[0]->color;
    root->children[1]->color = !root->children[1]->color;
}

RBTreeNode* Rotation(RBTreeNode* root, int route) {
    RBTreeNode* new_root = root->children[!route];
    root->children[!route] = new_root->children[route];

    new_root->children[route] = root;
    new_root->color = root->color;

    root->color = RED;

    return new_root;
}

RBTreeNode* DoubleRotation(RBTreeNode *root, int route) {
    root->children[!route] = Rotation(root->children[!route], !route);
    return Rotation(root, route);
}

RBTreeNode* BalanceRBTree(RBTreeNode* root, int route) {
    if (!(IsRed(root->children[route]))) {
        return root;
    }

    if (IsRed(root->children[route]) && IsRed(root->children[!route])) {
        if (IsRed(root->children[route]->children[route]) || IsRed(root->children[route]->children[!route])) {
                SwapColors(root);
        }
    } else {
        if (IsRed(root->children[route]->children[route])) {
            root = Rotation(root, !route);
        } else if (IsRed(root->children[route]->children[!route])) {
            root = DoubleRotation(root, !route);
        }
    }

    return root;
}

RBTreeNode* Insert(RBTreeNode* root, RBTreeNode* RBTree, int value, int index) {
    if (root == NULL) {
        RBTree[index].value = value;
        RBTree[index].color = RED;
        RBTree[index].children[LEFT] = NULL;
        RBTree[index].children[RIGHT] = NULL;
        root = &RBTree[index];
        return root;
    }

    int route = 0;
    if (value > root->value) {
        route = 1;
    }

    root->children[route] = Insert(root->children[route], RBTree, value, index);

    return BalanceRBTree(root, route);
}

RBTreeNode* CreateRBTree(RBTreeNode* root, int number_of_elements, RBTreeNode* RBTree) {
    for (int i = 0; i < number_of_elements; i++) {
        int value;
        if (!scanf("%d", &value)) {
            exit(EXIT_SUCCESS);
        }

        root = Insert(root, RBTree, value, i);
        root->color = BLACK;
    }

    return root;    
}

int GetHeight(RBTreeNode *root) {
    int height = 0;
    RBTreeNode* node = root;

    while (1) {
        if (node->color == BLACK) {
            height++;
        }

        node = node->children[RIGHT];

        if (node == NULL) {
            height++;
            break;
        }
    }
    return height;
}

int main() {
    int number_of_elements;
    if (!scanf("%d", &number_of_elements)) {
        return 0;
    }
    if (number_of_elements == 0) {
        printf("0");
        return 0;
    }

    RBTreeNode* RBTree = malloc(number_of_elements * sizeof(RBTreeNode));
    RBTreeNode* root = NULL;
    root = CreateRBTree(root, number_of_elements, RBTree);

    printf("%d", GetHeight(root));
    
    free(RBTree);

    return 0;
}
