#include "cuckooHash.h"

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

template class CuckooHash<string>;