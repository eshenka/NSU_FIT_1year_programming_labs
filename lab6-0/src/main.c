#include <stdio.h>
#include <malloc.h>

char max(char a, char b) {
    if (a > b) {
        return a;
    }
    return b;
}

typedef struct AVLTreeNode {
    int value;
    char height;
    struct AVLTreeNode* left;
    struct AVLTreeNode* right;
} AVLTreeNode;

char HeightNode(AVLTreeNode* node) {
    if (node != NULL) {
        return node->height;
    }
    return 0;
}

char GetHeight(AVLTreeNode* root) {
    return max(HeightNode(root->left), HeightNode(root->right)) + 1;
}

char HeightDifference(AVLTreeNode* left, AVLTreeNode* right) {
    return HeightNode(left) - HeightNode(right); 
}

AVLTreeNode* RightRotation(AVLTreeNode* root) {
    AVLTreeNode* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    root->height = GetHeight(root);
    new_root->height = GetHeight(new_root);
    return new_root;
}

AVLTreeNode* LeftRotation(AVLTreeNode* root) {
    AVLTreeNode* new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    root->height = GetHeight(root);
    new_root->height = GetHeight(new_root);
    return new_root;
}

AVLTreeNode* BalanceAVLTree(AVLTreeNode* root) {
    root->height = GetHeight(root);
    char height_difference = HeightDifference(root->left, root->right);
    if (height_difference >= 2) {
        if (root->left && HeightDifference(root->left->left, root->left->right) < 0) {
            root->left = LeftRotation(root->left);
        }
        return RightRotation(root);
    }
    if (height_difference <= -2) {
        if (root->right && HeightDifference(root->right->left, root->right->right) > 0) {
            root->right = RightRotation(root->right);
        }
        return LeftRotation(root);
    } 
    return root;
}

AVLTreeNode* Insert(AVLTreeNode* root, int value, AVLTreeNode* AVLTree, int index) {
    if (root == NULL) {
        AVLTree[index].value = value;
        AVLTree[index].height = 1;
        AVLTree[index].left = NULL;
        AVLTree[index].right = NULL;
        root = &AVLTree[index];
        return root;
    }
    if (value > root->value) {
        root->right = Insert(root->right, value, AVLTree, index);
    } else {
        root->left = Insert(root->left, value, AVLTree, index);
    }
    return BalanceAVLTree(root);
}

AVLTreeNode* CreateTree(AVLTreeNode* AVLTree, AVLTreeNode* root, int number_of_elements) {
    int value;
    for (int i = 0; i < number_of_elements; i++) {
        if (!scanf("%d", &value)) {
            return 0;
        }
        root = Insert(root, value, AVLTree, i);
    }
    return root;
}

int main() {
    int number_of_elements;
    if (!scanf("%d", &number_of_elements)) {
        return 0;
    }
    if (number_of_elements <= 0) {
        printf("0");
        return 0;
    }
    AVLTreeNode* AVLTree = (AVLTreeNode*) malloc(sizeof(AVLTreeNode) * number_of_elements);
    AVLTreeNode* root = NULL;
    root = CreateTree(AVLTree, root, number_of_elements);
    printf("%d", root->height);
    free(AVLTree);
    return 0;
}
