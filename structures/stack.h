#pragma once

#include <string>
#include <iostream>

class Stack {
private:
    struct Node {
        std::string text;
        Node* next;
    };
    Node* head;

public:
    Stack();
    ~Stack();
    void push(const std::string& value);
    void pop(std::string& outValue);
    bool isEmpty() const;
    void print() const;
    void clear();

    bool saveBinary(const std::string& filename) const;
    bool loadBinary(const std::string& filename);
};