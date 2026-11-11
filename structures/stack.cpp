#include "stack.h"

Stack::Stack() : head(nullptr) {}

Stack::~Stack() { clear(); }

void Stack::push(const std::string& value) {
    Node* newNode = new Node{value, head};
    head = newNode;
}

void Stack::pop(std::string& outValue) {
    if (head == nullptr) return;
    outValue = head->text;
    Node* temp = head;
    head = head->next;
    delete temp;
}

bool Stack::isEmpty() const { return head == nullptr; }

void Stack::print() const {
    if (!head) { std::cout << "Empty\n"; return; }
    Node* cur = head;
    while (cur) {
        std::cout << cur->text << " ";
        cur = cur->next;
    }
    std::cout << "\n";
}

void Stack::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}