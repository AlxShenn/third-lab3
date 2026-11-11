#include "array.h"
#include <fstream>
#include <cstdint>

void Array::resize() {
    size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
    string* newData = new string[newCapacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

Array::Array(size_t cap) : size(0), capacity(cap) {
    data = (capacity > 0) ? new string[capacity] : nullptr;
}

Array::~Array() { 
    delete[] data; 
}

void Array::addEnd(const string& text) {
    if (size == capacity) resize();
    data[size++] = text;
}

void Array::addIndex(const string& text, size_t index) {
    if (index > size) return;
    if (size == capacity) resize();
    for (size_t i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = text;
    size++;
}

string Array::get(size_t index) const {
    if (index >= size) return "";
    return data[index];
}

void Array::removeIndex(size_t index) {
    if (index >= size) return;
    for (size_t i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    size--;
}

size_t Array::length() const { 
    return size; 
}

void Array::print() const {
    for (size_t i = 0; i < size; ++i) {
        cout << data[i] << " ";
    }
    cout << "\n";
}

void Array::set(size_t index, const string& text) {
    if (index >= size) return;
    data[index] = text;
}

bool Array::saveBinary(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out) return false;

    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
        uint32_t len = static_cast<uint32_t>(data[i].size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(data[i].data(), len);
    }
    return static_cast<bool>(out);
}

bool Array::loadBinary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) return false;

    size_t newSize = 0;
    in.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));
    if (!in) return false;
    delete[] data;
    data = nullptr;
    capacity = newSize > 0 ? newSize : 0;
    data = (capacity > 0) ? new string[capacity] : nullptr;
    size = newSize;

    for (size_t i = 0; i < size; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (!in) return false;
        string s(len, '\0');
        if (len > 0) {
            in.read(&s[0], len);
            if (!in) return false;
        }
        data[i] = s;
    }
    return true;
}