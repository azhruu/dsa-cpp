#pragma once
#include <cstddef>
#include <stdexcept>
#include <string>

// Generic doubly linked list implemented from scratch (C++17).
// Each node points both ways, so insert/erase next to a known node and
// reverse traversal are O(1) / O(n) without rescanning from the head.
// Operations: push front/back, insert at index, erase value, find,
// reverse, forward and backward traversal.
template <typename T>
class DoublyLinkedList {
    struct Node {
        T data;
        Node* prev;
        Node* next;
        explicit Node(const T& d) : data(d), prev(nullptr), next(nullptr) {}
    };
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;

    // Node at index (0-based); walks from the nearer end.
    Node* nodeAt(std::size_t index) const {
        if (index >= size_) throw std::out_of_range("index out of range");
        Node* cur;
        if (index < size_ / 2) {
            cur = head_;
            for (std::size_t i = 0; i < index; ++i) cur = cur->next;
        } else {
            cur = tail_;
            for (std::size_t i = size_ - 1; i > index; --i) cur = cur->prev;
        }
        return cur;
    }

    void unlink(Node* n) {
        if (n->prev) n->prev->next = n->next; else head_ = n->next;
        if (n->next) n->next->prev = n->prev; else tail_ = n->prev;
        delete n;
        --size_;
    }

public:
    ~DoublyLinkedList() { clear(); }

    void pushFront(const T& value) {
        Node* n = new Node(value);
        n->next = head_;
        if (head_) head_->prev = n; else tail_ = n;
        head_ = n;
        ++size_;
    }

    void pushBack(const T& value) {
        Node* n = new Node(value);
        n->prev = tail_;
        if (tail_) tail_->next = n; else head_ = n;
        tail_ = n;
        ++size_;
    }

    // Insert before the element currently at index (index == size appends).
    void insert(std::size_t index, const T& value) {
        if (index > size_) throw std::out_of_range("index out of range");
        if (index == 0) { pushFront(value); return; }
        if (index == size_) { pushBack(value); return; }
        Node* at = nodeAt(index);
        Node* n = new Node(value);
        n->prev = at->prev;
        n->next = at;
        at->prev->next = n;
        at->prev = n;
        ++size_;
    }

    // Erase the first node holding value; returns false if absent.
    bool erase(const T& value) {
        for (Node* cur = head_; cur; cur = cur->next) {
            if (cur->data == value) { unlink(cur); return true; }
        }
        return false;
    }

    bool contains(const T& value) const {
        for (Node* cur = head_; cur; cur = cur->next)
            if (cur->data == value) return true;
        return false;
    }

    void reverse() {
        for (Node* cur = head_; cur; cur = cur->prev) {
            Node* tmp = cur->prev;
            cur->prev = cur->next;
            cur->next = tmp;
        }
        Node* tmp = head_;
        head_ = tail_;
        tail_ = tmp;
    }

    void clear() {
        while (head_) { Node* tmp = head_; head_ = head_->next; delete tmp; }
        tail_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    std::string toString() const {
        std::string out = "[";
        for (Node* cur = head_; cur; cur = cur->next) {
            out += std::to_string(cur->data);
            if (cur->next) out += " <-> ";
        }
        return out + "]";
    }

    // Same elements, walked tail -> head: proves the prev links work.
    std::string toStringReverse() const {
        std::string out = "[";
        for (Node* cur = tail_; cur; cur = cur->prev) {
            out += std::to_string(cur->data);
            if (cur->prev) out += " <-> ";
        }
        return out + "]";
    }
};
