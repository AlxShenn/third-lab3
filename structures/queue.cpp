#include "queue.h"
#include <iostream>
#include <fstream>
#include <cstdint>

Queue::Queue() : head(nullptr), tail(nullptr) {}

Queue::~Queue() { clear(); }

void Queue::push(const string& value) {
    Node* newNode = new Node{value, nullptr};
    if (!tail) {
        head = tail = newNode;
        return;
    }
    tail->next = newNode;
    tail = newNode;
}

void Queue::pop() {
    if (isEmpty()) {
        cout << "Queue is empty\n";
        return;
    }
    Node* temp = head;
    cout << "-> " << temp->text << "\n";
    head = head->next;
    if (!head) tail = nullptr;
    delete temp;
}

bool Queue::isEmpty() const {
    return head == nullptr;
}

void Queue::print() const {
    if (isEmpty()) return;
    Node* current = head;
    while (current) {
        cout << current->text;
        if (current->next) cout << " -> ";
        current = current->next;
    }
    cout << "\n";
}

void Queue::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
}

bool Queue::saveBinary(const string& filename) const {
    ofstream out(filename, ios::binary);
    if (!out) return false;

    uint32_t count = 0;
    for (Node* cur = head; cur; cur = cur->next) ++count;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (Node* cur = head; cur; cur = cur->next) {
        uint32_t len = static_cast<uint32_t>(cur->text.size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(cur->text.data(), len);
    }
    return static_cast<bool>(out);
}

bool Queue::loadBinary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) return false;

    clear();

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in) return false;

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (!in) return false;
        string s(len, '\0');
        if (len > 0) {
            in.read(&s[0], len);
            if (!in) return false;
        }
        push(s);
    }
    return true;
}