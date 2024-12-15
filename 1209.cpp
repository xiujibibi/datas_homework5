#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

// AVL樹節點
struct AVLNode {
    int data;
    AVLNode* left, * right;
    int height;
    AVLNode(int value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

// 2-3 樹節點
struct TwoThreeNode {
    int key1, key2;
    TwoThreeNode* left, * middle, * right;
    bool isTwoNode;
    TwoThreeNode(int value) : key1(value), key2(0), left(nullptr), middle(nullptr), right(nullptr), isTwoNode(true) {}
};

// AVL樹類別
class AVLTree {
public:
    AVLNode* root;
    AVLTree() : root(nullptr) {}

    // 插入節點
    AVLNode* insert(AVLNode* node, int data) {
        if (!node) return new AVLNode(data);

        if (data < node->data)
            node->left = insert(node->left, data);
        else if (data > node->data)
            node->right = insert(node->right, data);
        else
            return node; // 重複元素不處理

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        // 進行旋轉
        if (balance > 1 && data < node->left->data)
            return rightRotate(node);
        if (balance < -1 && data > node->right->data)
            return leftRotate(node);
        if (balance > 1 && data > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && data < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // 刪除節點
    AVLNode* deleteNode(AVLNode* root, int key) {
        if (!root) return root;

        // 1. 找到節點
        if (key < root->data)
            root->left = deleteNode(root->left, key);
        else if (key > root->data)
            root->right = deleteNode(root->right, key);
        else {
            // 找到節點後處理刪除
            if (!root->left || !root->right) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp;
                }
                delete temp;
            }
            else {
                AVLNode* temp = minValueNode(root->right);
                root->data = temp->data;
                root->right = deleteNode(root->right, temp->data);
            }
        }

        if (!root) return root;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        int balance = getBalance(root);

        // 進行旋轉
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    // 右旋轉
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    // 左旋轉
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    int getHeight(AVLNode* node) {
        if (!node) return 0;
        return node->height;
    }

    int getBalance(AVLNode* node) {
        if (!node) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    // 顯示樹結構，使用 '-' 來顯示父子關係
    void printTree(AVLNode* root, string prefix = "", bool isLeft = true) {
        if (root != nullptr) {
            cout << prefix;
            cout << (isLeft ? "|-- " : "|__ ") << root->data << endl;
            printTree(root->left, prefix + (isLeft ? "|   " : "    "), true);
            printTree(root->right, prefix + (isLeft ? "|   " : "    "), false);
        }
    }

    void insert(int data) {
        root = insert(root, data);
    }

    void deleteNode(int key) {
        root = deleteNode(root, key);
    }

    // 顯示AVL樹的樹狀結構
    void display() {
        cout << "AVL樹的樹狀結構：\n";
        printTree(root);
        cout << endl;
    }
};

// 2-3樹類別
class TwoThreeTree {
public:
    TwoThreeNode* root;
    TwoThreeTree() : root(nullptr) {}

    // 插入節點
    TwoThreeNode* insert(TwoThreeNode* node, int data) {
        if (!node) return new TwoThreeNode(data);

        // 這裡簡單處理節點插入，不涉及分裂邏輯，只顯示結構
        if (node->isTwoNode) {
            if (data < node->key1) {
                node->key2 = node->key1;
                node->key1 = data;
            }
            else {
                node->key2 = data;
            }
            node->isTwoNode = false;
        }
        else {
            // 處理三個節點的情況
            if (data < node->key1) {
                node->key2 = node->key1;
                node->key1 = data;
            }
            else if (data > node->key2) {
                node->key2 = data;
            }
        }
        return node;
    }

    // 顯示2-3樹結構，處理所有層次
    void printTree(TwoThreeNode* root, string prefix = "", bool isLeft = true) {
        if (root != nullptr) {
            cout << prefix;
            cout << (isLeft ? "|-- " : "|__ ") << root->key1;
            if (!root->isTwoNode) {
                cout << "," << root->key2;
            }
            cout << endl;

            if (root->left) {
                printTree(root->left, prefix + (isLeft ? "|   " : "    "), true);
            }
            if (root->middle) {
                printTree(root->middle, prefix + (isLeft ? "|   " : "    "), false);
            }
            if (root->right) {
                printTree(root->right, prefix + (isLeft ? "|   " : "    "), false);
            }
        }
    }

    void insert(int data) {
        root = insert(root, data);
    }

    void deleteNode(int key) {
        // 2-3 樹刪除的邏輯需要實現，這裡略過
    }

    // 顯示2-3樹的樹狀結構
    void display() {
        cout << "2-3樹的樹狀結構：\n";
        printTree(root);
        cout << endl;
    }
};

int main() {
    AVLTree avl;
    TwoThreeTree twoThree;
    int n, value, op;

    cout << "請輸入節點數量: ";
    cin >> n;

    cout << "請輸入 " << n << " 個整數: ";
    for (int i = 0; i < n; i++) {
        cin >> value;
        avl.insert(value);
        twoThree.insert(value);
    }

    avl.display();
    twoThree.display();

    // 反覆執行插入或刪除操作
    while (true) {
        cout << "請選擇操作 (1: 插入, 2: 刪除, 0: 結束): ";
        cin >> op;

        if (op == 0) {
            break;  // 結束操作
        }

        cout << "請輸入資料: ";
        cin >> value;

        if (op == 1) {
            avl.insert(value);
            twoThree.insert(value);
        }
        else if (op == 2) {
            avl.deleteNode(value);
            twoThree.deleteNode(value);
        }

        avl.display();
        twoThree.display();
    }

    return 0;
}
