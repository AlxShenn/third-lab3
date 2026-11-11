#include "cuckooHash.h"
#include <fstream>
#include <vector>
#include <cstdint>

template <typename T>
size_t CuckooHash<T>::hash1(const T& key) const {
    return std::hash<T>{}(key) % capacity;
}

template <typename T>
size_t CuckooHash<T>::hash2(const T& key) const {
    return (std::hash<T>{}(key) / capacity + 1) % capacity;
}

template <typename T>
void CuckooHash<T>::rehash() {
    Entry* old1 = table1;
    Entry* old2 = table2;
    size_t oldCapacity = capacity;
    
    capacity *= 2;
    table1 = new Entry[capacity];
    table2 = new Entry[capacity];
    size = 0;
    
    for (size_t i = 0; i < oldCapacity; ++i) {
        if (old1[i].occupied) add(old1[i].key);
        if (old2[i].occupied) add(old2[i].key);
    }

    delete[] old1;
    delete[] old2;
}

template <typename T>
CuckooHash<T>::CuckooHash(size_t cap) : capacity(cap), size(0) {
    table1 = new Entry[capacity];
    table2 = new Entry[capacity];
}

template <typename T>
CuckooHash<T>::~CuckooHash() {
    delete[] table1;
    delete[] table2;
}

template <typename T>
void CuckooHash<T>::add(const T& key) {
    if (contains(key)) return;
    
    T currentKey = key;
    for (int i = 0; i < 100; ++i) {
        size_t pos1 = hash1(currentKey);
        if (!table1[pos1].occupied) {
            table1[pos1].key = currentKey;
            table1[pos1].occupied = true;
            size++;
            return;
        }
        T evicted = table1[pos1].key;
        table1[pos1].key = currentKey;
        currentKey = evicted;

        size_t pos2 = hash2(currentKey);
        if (!table2[pos2].occupied) {
            table2[pos2].key = currentKey;
            table2[pos2].occupied = true;
            size++;
            return;
        }
        evicted = table2[pos2].key;
        table2[pos2].key = currentKey;
        currentKey = evicted;
    }
    rehash();
    add(currentKey);
}

template <typename T>
bool CuckooHash<T>::contains(const T& key) const {
    size_t pos1 = hash1(key);
    if (table1[pos1].occupied && table1[pos1].key == key) return true;
    
    size_t pos2 = hash2(key);
    if (table2[pos2].occupied && table2[pos2].key == key) return true;
    
    return false;
}

template <typename T>
bool CuckooHash<T>::saveBinary(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out) return false;

    uint64_t cap = static_cast<uint64_t>(capacity);
    out.write(reinterpret_cast<const char*>(&cap), sizeof(cap));

    std::vector<T> all;
    all.reserve(size);
    for (size_t i = 0; i < capacity; ++i) {
        if (table1[i].occupied) all.push_back(table1[i].key);
        if (table2[i].occupied) all.push_back(table2[i].key);
    }

    uint64_t cnt = static_cast<uint64_t>(all.size());
    out.write(reinterpret_cast<const char*>(&cnt), sizeof(cnt));

    for (const T& key : all) {
        uint32_t len = static_cast<uint32_t>(key.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(key.data(), len);
    }
    return static_cast<bool>(out);
}

template <typename T>
bool CuckooHash<T>::loadBinary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) return false;

    uint64_t cap = 0, cnt = 0;
    in.read(reinterpret_cast<char*>(&cap), sizeof(cap));
    in.read(reinterpret_cast<char*>(&cnt), sizeof(cnt));
    if (!in) return false;

    delete[] table1;
    delete[] table2;
    capacity = (cap > 0) ? static_cast<size_t>(cap) : 1;
    size = 0;
    table1 = new Entry[capacity];
    table2 = new Entry[capacity];

    for (uint64_t i = 0; i < cnt; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (!in) return false;
        T key;
        key.resize(len);
        if (len > 0) {
            in.read(&key[0], len);
            if (!in) return false;
        }
        add(key);
    }
    return true;
}

template class CuckooHash<string>;