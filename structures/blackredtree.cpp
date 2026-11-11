#include "blackredtree.h"
#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

RedBlackTree::RedBlackTree() : root(nullptr) {}

RedBlackTree::~RedBlackTree() {
    deleteTree(root);
}

void RedBlackTree::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void RedBlackTree::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void RedBlackTree::insert(int value) {
    Node* z = new Node(value, true, nullptr);
    Node* y = nullptr;
    Node* x = root;

    while (x) {
        y = x;
        if (z->data < x->data) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (!y) root = z;
    else if (z->data < y->data) y->left = z;
    else y->right = z;

    Insert(z);
}

void RedBlackTree::Insert(Node* z) {
    while (z->parent && z->parent->isRed) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y && y->isRed) {
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                rotateRight(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y && y->isRed) {
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->isRed = false;
}

void RedBlackTree::transplant(Node* u, Node* v) {
    if (!u->parent) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}

RedBlackTree::Node* RedBlackTree::findMin(Node* node) const {
    while (node && node->left) node = node->left;
    return node;
}

void RedBlackTree::remove(int value) {
    Node* z = root;
    while (z && z->data != value) {
        if (value < z->data) z = z->left;
        else z = z->right;
    }
    if (!z) return;

    Node* y = z;
    bool yOriginalRed = y->isRed;
    Node* x;

    if (!z->left) {
        x = z->right;
        transplant(z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = findMin(z->right);
        yOriginalRed = y->isRed;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->isRed = z->isRed;
    }

    delete z;

    if (!yOriginalRed) {
        Delete(x);
    }
}

void RedBlackTree::Delete(Node* x) {
    while (x != root && (!x || !x->isRed)) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w && w->isRed) {
                w->isRed = false;
                x->parent->isRed = true;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if ((!w->left || !w->left->isRed) && (!w->right || !w->right->isRed)) {
                if (w) w->isRed = true;
                x = x->parent;
            } else {
                if (!w->right || !w->right->isRed) {
                    if (w->left) w->left->isRed = false;
                    if (w) w->isRed = true;
                    rotateRight(w);
                    w = x->parent->right;
                }
                if (w) w->isRed = x->parent->isRed;
                x->parent->isRed = false;
                if (w->right) w->right->isRed = false;
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            Node* w = x->parent->left;
            if (w && w->isRed) {
                w->isRed = false;
                x->parent->isRed = true;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if ((!w->right || !w->right->isRed) && (!w->left || !w->left->isRed)) {
                if (w) w->isRed = true;
                x = x->parent;
            } else {
                if (!w->left || !w->left->isRed) {
                    if (w->right) w->right->isRed = false;
                    if (w) w->isRed = true;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                if (w) w->isRed = x->parent->isRed;
                x->parent->isRed = false;
                if (w->left) w->left->isRed = false;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    if (x) x->isRed = false;
}

bool RedBlackTree::search(int target) const {
    Node* current = root;
    while (current) {
        if (target == current->data) return true;
        if (target < current->data) current = current->left;
        else current = current->right;
    }
    return false;
}

void RedBlackTree::printInOrder(Node* node) const {
    if (!node) return;
    printInOrder(node->left);
    cout << node->data << " ";
    printInOrder(node->right);
}

void RedBlackTree::print() const {
    printInOrder(root);
    cout << "\n";
}

void RedBlackTree::clear() {
    deleteTree(root);
    root = nullptr;
}

void RedBlackTree::saveInOrder(Node* node, void* outPtr) const {
    if (!node) return;
    saveInOrder(node->left, outPtr);
    ofstream& out = *((ofstream*)outPtr);
    int value = node->data;
    out.write((const char*)&value, sizeof(value));
    saveInOrder(node->right, outPtr);
}

void RedBlackTree::loadInOrder(int value) {
    insert(value);
}

bool RedBlackTree::saveBinary(const char* filename) const {
    ofstream out(filename, ios::binary);
    if (!out) return false;

    uint64_t count = 0;
    
    Node** stack = new Node*[100];
    int stackCapacity = 100;
    int stackSize = 0;
    Node* cur = root;

    while (cur || stackSize > 0) {
        while (cur) {
            if (stackSize == stackCapacity) {
                stackCapacity *= 2;
                Node** newStack = new Node*[stackCapacity];
                for (int i = 0; i < stackSize; ++i) newStack[i] = stack[i];
                delete[] stack;
                stack = newStack;
            }
            stack[stackSize++] = cur;
            cur = cur->left;
        }
        cur = stack[--stackSize];
        ++count;
        cur = cur->right;
    }
    delete[] stack;

    out.write((const char*)&count, sizeof(count));
    saveInOrder(root, &out);
    return (bool)out;
}

bool RedBlackTree::loadBinary(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in) return false;

    uint64_t count = 0;
    in.read((char*)&count, sizeof(count));
    if (!in) return false;

    clear();

    for (uint64_t i = 0; i < count; ++i) {
        int value = 0;
        in.read((char*)&value, sizeof(value));
        if (!in) return false;
        insert(value);
    }
    return true;
}