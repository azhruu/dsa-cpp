#pragma once
#include <cstddef>
#include <stdexcept>
#include <string>

// Generic singly linked list implemented from scratch (C++17).
// Operations: push front/back, insert at index, erase value, find, reverse.
template <typename T>
class SinglyLinkedList {
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* head_ = nullptr;
    std::size_t size_ = 0;

public:
    ~SinglyLinkedList() { clear(); }

    void pushFront(const T& value) {
        Node* n = new Node(value);
        n->next = head_;
        head_ = n;
        ++size_;
    }

    void pushBack(const T& value) {
        Node* n = new Node(value);
        if (!head_) { head_ = n; ++size_; return; }
        Node* cur = head_;
        while (cur->next) cur = cur->next;
        cur->next = n;
        ++size_;
    }

    bool erase(const T& value) {
        if (!head_) return false;
        if (head_->data == value) {
            Node* tmp = head_; head_ = head_->next; delete tmp; --size_; return true;
        }
        Node* cur = head_;
        while (cur->next && !(cur->next->data == value)) cur = cur->next;
        if (!cur->next) return false;
        Node* tmp = cur->next; cur->next = tmp->next; delete tmp; --size_;
        return true;
    }

    bool contains(const T& value) const {
        for (Node* cur = head_; cur; cur = cur->next)
            if (cur->data == value) return true;
        return false;
    }

    void reverse() {
        Node* prev = nullptr; Node* cur = head_;
        while (cur) { Node* nxt = cur->next; cur->next = prev; prev = cur; cur = nxt; }
        head_ = prev;
    }

    void clear() {
        while (head_) { Node* tmp = head_; head_ = head_->next; delete tmp; }
        size_ = 0;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    std::string toString() const {
        std::string out = "[";
        for (Node* cur = head_; cur; cur = cur->next) {
            out += std::to_string(cur->data);
            if (cur->next) out += " -> ";
        }
        return out + "]";
    }
};
