#pragma once

#include <string>

using namespace std;

class Queue {
private:
    struct Node {
        string text;
        Node* next;
    };
    Node* head;
    Node* tail;

public:
    Queue();
    ~Queue();

    void push(const string& value);
    void pop();
    bool isEmpty() const;
    void print() const;
    void clear();

    // Бинарная сериализация
    bool saveBinary(const string& filename) const;
    bool loadBinary(const string& filename);
};