#include "slist.h"
#include <iostream>

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