#include "array.h"

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
    if (index >= size) return ""; // Защита от выхода за границы
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