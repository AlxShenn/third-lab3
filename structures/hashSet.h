#pragma once

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class HashSet {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* n) : data(val), next(n) {}
    };
    
    Node** table;
    size_t capacity;
    size_t size;

    size_t hash(const T& key) const;

public:
    HashSet(size_t cap = 1000);
    ~HashSet();
    void free();
    void add(const T& value);
    bool remove(const T& value);
    bool contains(const T& value) const;
    size_t getSize() const;
    T* getAll() const; 
};
