#pragma once
#include <cstddef>
#include <stdexcept>
#include <vector>

// Generic LIFO stack on a dynamically growing array (C++17).
template <typename T>
class ArrayStack {
    std::vector<T> data_;

public:
    void push(const T& value) { data_.push_back(value); }

    T pop() {
        if (empty()) throw std::runtime_error("stack is empty");
        T v = data_.back();
        data_.pop_back();
        return v;
    }

    const T& peek() const {
        if (empty()) throw std::runtime_error("stack is empty");
        return data_.back();
    }

    std::size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
};
