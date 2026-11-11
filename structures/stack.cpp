#include "stack.h"
#include <fstream>
#include <cstdint>

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

bool Stack::saveBinary(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
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

bool Stack::loadBinary(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    clear();

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in) return false;

    Node* tempHead = nullptr;

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t len = 0;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (!in) {

            while (tempHead) { Node* t = tempHead; tempHead = tempHead->next; delete t; }
            return false;
        }
        std::string s(len, '\0');
        if (len > 0) {
            in.read(&s[0], len);
            if (!in) {
                while (tempHead) { Node* t = tempHead; tempHead = tempHead->next; delete t; }
                return false;
            }
        }

        tempHead = new Node{s, tempHead};
    }

    Node* cur = tempHead;
    while (cur) {
        push(cur->text);
        cur = cur->next;
    }

    while (tempHead) {
        Node* t = tempHead;
        tempHead = tempHead->next;
        delete t;
    }

    return true;
}