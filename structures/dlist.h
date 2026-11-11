#pragma once

#include <string>

using namespace std;

class Dlist {
private:
    struct Node {
        string text;
        Node* next;
        Node* prev;
    };
    Node* head;
    Node* tail;

public:
    Dlist();
    ~Dlist();

    void pushHead(const string& value);
    void pushTail(const string& value);
    void pushAfter(const string& target, const string& value);
    void pushBefore(const string& target, const string& value);

    void deleteHead();
    void deleteTail();
    void deleteAfter(const string& target);
    void deleteBefore(const string& target);
    void deleteValue(const string& value);

    bool find(const string& value) const;
    void printForward() const;
    void printBackward() const;
    void clear();

    bool saveBinary(const string& filename) const;
    bool loadBinary(const string& filename);
};