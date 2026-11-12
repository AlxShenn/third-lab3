#pragma once

class RedBlackTree {
private:
    struct Node {
        int data;
        bool isRed;
        Node* left;
        Node* right;
        Node* parent;
        Node(int val, bool red, Node* p) : data(val), isRed(red), left(nullptr), right(nullptr), parent(p) {}
    };

    Node* root;

    void rotateLeft(Node* x);
    void rotateRight(Node* y);
    void Insert(Node* z);
    void deleteTree(Node* node);
    void transplant(Node* u, Node* v);
    void Delete(Node* x, Node* parent);
    Node* findMin(Node* node) const;

    void printInOrder(Node* node) const;
    void saveInOrder(Node* node, void* out) const;
    void loadInOrder(int value);

public:
    RedBlackTree();
    ~RedBlackTree();

    void insert(int value);
    void remove(int value);
    bool search(int target) const;
    void print() const;
    void clear();

    bool saveBinary(const char* filename) const;
    bool loadBinary(const char* filename);
};