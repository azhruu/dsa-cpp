#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Binary heap implemented from scratch (C++17) on a plain array: the
// parent of index i sits at (i - 1) / 2, its children at 2i + 1 and 2i + 2.
// The Compare functor decides which element has priority: std::less<T>
// makes a max-heap (largest on top), std::greater<T> a min-heap.
// Operations: push, pop (removes and returns the top), top, size, empty,
// heapify-from-vector construction, an isHeap check, and heapSort.
// push/pop run in O(log n); building from a vector runs in O(n).
template <typename T, typename Compare = std::less<T>>
class Heap {
    std::vector<T> data_;
    Compare comp_;

    // True when child outranks parent, i.e. the heap property is broken.
    bool outranks(std::size_t child, std::size_t parent) const {
        return comp_(data_[parent], data_[child]);
    }

    void siftUp(std::size_t i) {
        while (i > 0) {
            std::size_t parent = (i - 1) / 2;
            if (!outranks(i, parent)) break;
            std::swap(data_[i], data_[parent]);
            i = parent;
        }
    }

    void siftDown(std::size_t i) {
        const std::size_t n = data_.size();
        while (true) {
            std::size_t best = i;
            std::size_t left = 2 * i + 1, right = 2 * i + 2;
            if (left < n && outranks(left, best)) best = left;
            if (right < n && outranks(right, best)) best = right;
            if (best == i) break;
            std::swap(data_[i], data_[best]);
            i = best;
        }
    }

    void heapify() {
        if (data_.empty()) return;
        // Sift down from the last parent: every subtree below is already
        // a heap, so one pass restores the whole array in O(n).
        for (std::size_t i = data_.size() / 2; i-- > 0;) siftDown(i);
    }

public:
    Heap() = default;
    explicit Heap(Compare comp) : comp_(comp) {}

    // Build a heap from existing values in O(n) (heapify, not n pushes).
    explicit Heap(std::vector<T> values, Compare comp = Compare())
        : data_(std::move(values)), comp_(comp) {
        heapify();
    }

    void push(const T& value) {
        data_.push_back(value);
        siftUp(data_.size() - 1);
    }

    // Removes and returns the top element; throws on an empty heap.
    T pop() {
        if (empty()) throw std::runtime_error("heap is empty");
        T top = data_.front();
        data_.front() = data_.back();
        data_.pop_back();
        if (!empty()) siftDown(0);
        return top;
    }

    const T& top() const {
        if (empty()) throw std::runtime_error("heap is empty");
        return data_.front();
    }

    void clear() { data_.clear(); }

    std::size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }

    // True when every parent outranks (or equals) its children.
    bool isHeap() const {
        for (std::size_t i = 1; i < data_.size(); ++i) {
            if (outranks(i, (i - 1) / 2)) return false;
        }
        return true;
    }

    // The heap as a flat array (level order), for inspection and tests.
    const std::vector<T>& toVector() const { return data_; }

    std::string toString() const {
        std::string out = "[";
        for (std::size_t i = 0; i < data_.size(); ++i) {
            out += std::to_string(data_[i]);
            if (i + 1 < data_.size()) out += ", ";
        }
        return out + "]";
    }

    // Heap sort: build a max-heap, then repeatedly take the largest
    // remaining element. Returns the values in ascending order.
    static std::vector<T> heapSort(std::vector<T> values) {
        Heap<T, std::less<T>> heap(std::move(values));
        std::vector<T> sorted;
        sorted.reserve(heap.size());
        while (!heap.empty()) sorted.push_back(heap.pop());  // descending
        std::reverse(sorted.begin(), sorted.end());           // ascending
        return sorted;
    }
};

// Max-heap: the largest element is always on top.
template <typename T>
using MaxHeap = Heap<T, std::less<T>>;

// Min-heap: the smallest element is always on top.
template <typename T>
using MinHeap = Heap<T, std::greater<T>>;
