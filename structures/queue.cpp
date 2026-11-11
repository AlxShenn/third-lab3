#include "queue.h"
#include <iostream>

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