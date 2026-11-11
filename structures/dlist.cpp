#include "dlist.h"
#include <iostream>
#include <fstream>
#include <cstdint>

Dlist::Dlist() : head(nullptr), tail(nullptr) {}

Dlist::~Dlist() { clear(); }

void Dlist::pushHead(const string& value) {
    Node* newNode = new Node{value, head, nullptr};
    if (!head) {
        head = tail = newNode;
    } else {
        head->prev = newNode;
        head = newNode;
    }
}

void Dlist::pushTail(const string& value) {
    Node* newNode = new Node{value, nullptr, tail};
    if (!tail) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

void Dlist::pushAfter(const string& target, const string& value) {
    Node* current = head;
    while (current && current->text != target) current = current->next;
    if (current) {
        Node* newNode = new Node{value, current->next, current};
        if (current->next) current->next->prev = newNode;
        else tail = newNode;
        current->next = newNode;
    }
}

void Dlist::pushBefore(const string& target, const string& value) {
    Node* current = head;
    while (current && current->text != target) current = current->next;
    if (current) {
        Node* newNode = new Node{value, current, current->prev};
        if (current->prev) current->prev->next = newNode;
        else head = newNode;
        current->prev = newNode;
    }
}

void Dlist::deleteHead() {
    if (!head) return;
    Node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    else tail = nullptr;
    delete temp;
}

void Dlist::deleteTail() {
    if (!tail) return;
    Node* temp = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else head = nullptr;
    delete temp;
}

void Dlist::deleteAfter(const string& target) {
    Node* current = head;
    while (current && current->text != target) current = current->next;
    if (current && current->next) {
        Node* temp = current->next;
        current->next = temp->next;
        if (temp->next) temp->next->prev = current;
        else tail = current;
        delete temp;
    }
}

void Dlist::deleteBefore(const string& target) {
    Node* current = head;
    while (current && current->text != target) current = current->next;
    if (current && current->prev) {
        Node* temp = current->prev;
        current->prev = temp->prev;
        if (temp->prev) temp->prev->next = current;
        else head = current;
        delete temp;
    }
}

void Dlist::deleteValue(const string& value) {
    Node* current = head;
    while (current && current->text != value) current = current->next;
    if (current) {
        if (current->prev) current->prev->next = current->next;
        else head = current->next;
        if (current->next) current->next->prev = current->prev;
        else tail = current->prev;
        delete current;
    }
}

bool Dlist::find(const string& value) const {
    Node* current = head;
    while (current) {
        if (current->text == value) return true;
        current = current->next;
    }
    return false;
}

void Dlist::printForward() const {
    if (!head) {
        cout << "Dlist is empty\n";
        return;
    }
    Node* current = head;
    while (current) {
        cout << current->text << " ";
        current = current->next;
    }
    cout << "\n";
}

void Dlist::printBackward() const {
    if (!tail) {
        cout << "Dlist is empty\n";
        return;
    }
    Node* current = tail;
    while (current) {
        cout << current->text << " ";
        current = current->prev;
    }
    cout << "\n";
}

void Dlist::clear() {
    while (head) {
        deleteHead();
    }
}

bool Dlist::saveBinary(const string& filename) const {
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

bool Dlist::loadBinary(const string& filename) {
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
        pushTail(s);
    }
    return true;
}