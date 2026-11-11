#pragma once

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class CuckooHash {
private:
    struct Entry {
        T key;
        bool occupied;
        Entry() : occupied(false) {}
    };
    
    Entry* table1;
    Entry* table2;
    size_t capacity;
    size_t size;

    size_t hash1(const T& key) const;
    size_t hash2(const T& key) const;
    void rehash();

public:
    CuckooHash(size_t cap = 100);
    ~CuckooHash();
    void add(const T& key);
    bool contains(const T& key) const;
};
