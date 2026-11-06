#pragma once
#include <cstddef>
#include <stdexcept>
#include <vector>

// Generic FIFO queue on a circular buffer (C++17).
template <typename T>
class CircularQueue {
    std::vector<T> buf_;
    std::size_t head_ = 0, size_ = 0;

public:
    explicit CircularQueue(std::size_t capacity = 8) : buf_(capacity) {}

    void enqueue(const T& value) {
        if (size_ == buf_.size()) grow();
        buf_[(head_ + size_) % buf_.size()] = value;
        ++size_;
    }

    T dequeue() {
        if (empty()) throw std::runtime_error("queue is empty");
        T v = buf_[head_];
        head_ = (head_ + 1) % buf_.size();
        --size_;
        return v;
    }

    const T& peek() const {
        if (empty()) throw std::runtime_error("queue is empty");
        return buf_[head_];
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

private:
    void grow() {
        std::vector<T> bigger(buf_.size() * 2);
        for (std::size_t i = 0; i < size_; ++i) bigger[i] = buf_[(head_ + i) % buf_.size()];
        buf_ = std::move(bigger);
        head_ = 0;
    }
};
