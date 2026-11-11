#include "hashSet.h"
#include <fstream>
#include <cstdint>

template <typename T>
size_t HashSet<T>::hash(const T& key) const {
    return std::hash<T>{}(key) % capacity;
}

template <typename T>
HashSet<T>::HashSet(size_t cap) : capacity(cap), size(0) {
    table = new Node*[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

template <typename T>
HashSet<T>::~HashSet() { 
    free(); 
    delete[] table; 
}

template <typename T>
void HashSet<T>::free() {
    for (size_t i = 0; i < capacity; ++i) {
        Node* cur = table[i];
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        table[i] = nullptr;
    }
    size = 0;
}

template <typename T>
void HashSet<T>::add(const T& value) {
    size_t idx = hash(value);
    Node* cur = table[idx];
    while (cur) {
        if (cur->data == value) return;
        cur = cur->next;
    }
    table[idx] = new Node(value, table[idx]);
    size++;
}

template <typename T>
bool HashSet<T>::remove(const T& value) {
    size_t idx = hash(value);
    Node* cur = table[idx];
    Node* prev = nullptr;
    while (cur) {
        if (cur->data == value) {
            if (prev) prev->next = cur->next;
            else table[idx] = cur->next;
            delete cur;
            size--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

template <typename T>
bool HashSet<T>::contains(const T& value) const {
    size_t idx = hash(value);
    Node* cur = table[idx];
    while (cur) {
        if (cur->data == value) return true;
        cur = cur->next;
    }
    return false;
}

template <typename T>
size_t HashSet<T>::getSize() const { return size; }

template <typename T>
T* HashSet<T>::getAll() const {
    if (size == 0) return nullptr;

    T* result = new T[size];
    size_t index = 0;

    for (size_t i = 0; i < capacity; ++i) {
        Node* cur = table[i];
        while (cur) {
            result[index++] = cur->data;
            cur = cur->next;
        }
    }
    return result;
}

template <typename T>
bool HashSet<T>::saveBinary(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out) return false;

    uint64_t cap = static_cast<uint64_t>(capacity);
    uint64_t sz = static_cast<uint64_t>(size);
    out.write(reinterpret_cast<const char*>(&cap), sizeof(cap));
    out.write(reinterpret_cast<const char*>(&sz), sizeof(sz));

    for (size_t i = 0; i < capacity; ++i) {
        for (Node* cur = table[i]; cur; cur = cur->next) {
            uint32_t len = static_cast<uint32_t>(cur->data.size());
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(cur->data.data(), len);
        }
    }
    return static_cast<bool>(out);
}

template <typename T>
bool HashSet<T>::loadBinary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) return false;

    uint64_t cap = 0, sz = 0;
    in.read(reinterpret_cast<char*>(&cap), sizeof(cap));
    in.read(reinterpret_cast<char*>(&sz), sizeof(sz));
    if (!in) return false;

    free();
    delete[] table;
    capacity = (cap > 0) ? static_cast<size_t>(cap) : 1;
    size = 0;
    table = new Node*[capacity];
    for (size_t i = 0; i < capacity; ++i) table[i] = nullptr;

    for (uint64_t i = 0; i < sz; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (!in) return false;
        string s(len, '\0');
        if (len > 0) {
            in.read(&s[0], len);
            if (!in) return false;
        }
        add(s);
    }
    return true;
}

template class HashSet<string>;