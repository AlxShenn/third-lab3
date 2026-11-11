#pragma once

#include <iostream>
#include <string>

using namespace std;

class Array {
private:
    string* data;
    size_t size;
    size_t capacity;

    void resize();

public:
    Array(size_t cap = 0);
    ~Array();

    void addEnd(const string& text);
    void addIndex(const string& text, size_t index);
    string get(size_t index) const;
    void removeIndex(size_t index);
    size_t length() const;
    void print() const;
    void set(size_t index, const string& text);
    bool saveBinary(const string& filename) const;
    bool loadBinary(const string& filename);
};