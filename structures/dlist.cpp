#include "dlist.h"
#include <iostream>

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