#include "slist.h"
#include <iostream>
#include <fstream>
#include <cstdint>

Slist::Slist() : head(nullptr) {}

Slist::~Slist() { clear(); }

void Slist::pushHead(const string& value) {
    Node* newNode = new Node{value, head};
    head = newNode;
}

void Slist::pushTail(const string& value) {
    if (!head) {
        pushHead(value);
        return;
    }
    Node* current = head;
    while (current->next) current = current->next;
    current->next = new Node{value, nullptr};
}

void Slist::pushAfter(const string& target, const string& value) {
    Node* current = head;
    while (current && current->text != target) current = current->next;
    if (current) {
        current->next = new Node{value, current->next};
    }
}

void Slist::pushBefore(const string& target, const string& value) {
    if (!head) return;
    if (head->text == target) {
        pushHead(value);
        return;
    }
    Node* current = head;
    while (current->next && current->next->text != target) current = current->next;
    if (current->next) {
        current->next = new Node{value, current->next};
    }
}

void Slist::deleteHead() {
    if (!head) return;
    Node* temp = head;
    head = head->next;
    delete temp;
}

void Slist::deleteTail() {
    if (!head) return;
    if (!head->next) {
        deleteHead();
        return;
    }
    Node* current = head;
    while (current->next->next) current = current->next;
    delete current->next;
    current->next = nullptr;
}

void Slist::deleteAfter(const string& target) {
    Node* current = head;
    while (current && current->text != target) current = current->next;
    if (current && current->next) {
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
    }
}

void Slist::deleteBefore(const string& target) {
    if (!head || !head->next) return;
    if (head->next->text == target) {
        deleteHead();
        return;
    }
    Node* current = head;
    while (current->next->next && current->next->next->text != target) {
        current = current->next;
    }
    if (current->next->next) {
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
    }
}

void Slist::deleteValue(const string& value) {
    if (!head) return;
    if (head->text == value) {
        deleteHead();
        return;
    }
    Node* current = head;
    while (current->next && current->next->text != value) current = current->next;
    if (current->next) {
        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
    }
}

bool Slist::find(const string& value) const {
    Node* current = head;
    while (current) {
        if (current->text == value) return true;
        current = current->next;
    }
    return false;
}

void Slist::print() const {
    if (!head) {
        cout << "Slist is empty\n";
        return;
    }
    Node* current = head;
    while (current) {
        cout << current->text << " ";
        current = current->next;
    }
    cout << "\n";
}

void Slist::clear() {
    while (head) {
        deleteHead();
    }
}

bool Slist::saveBinary(const string& filename) const {
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

bool Slist::loadBinary(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) return false;

    clear();

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in) return false;

    Node* tail = nullptr;
    for (uint32_t i = 0; i < count; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (!in) return false;
        string s(len, '\0');
        if (len > 0) {
            in.read(&s[0], len);
            if (!in) return false;
        }
        Node* node = new Node{s, nullptr};
        if (!head) head = node;
        else tail->next = node;
        tail = node;
    }
    return true;
}