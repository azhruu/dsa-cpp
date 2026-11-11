#pragma once
#include <cstddef>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

// Generic hash table with separate chaining, implemented from scratch
// (C++17). Keys hash into buckets; collisions share a bucket as a
// singly linked chain. The table rehashes (doubles its bucket count)
// when the load factor passes 0.75, keeping lookups near O(1).
// insert: true for a new key, false when an existing key's value is
// updated. get: true + value out-param on a hit, false on a miss.
template <typename K, typename V, typename Hash = std::hash<K>>
class HashTable {
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    std::vector<Node*> buckets_;
    std::size_t size_ = 0;
    Hash hash_;

    static constexpr double MAX_LOAD_FACTOR = 0.75;

    std::size_t bucketIndex(const K& key) const {
        return hash_(key) % buckets_.size();
    }

    void rehash(std::size_t newBucketCount) {
        std::vector<Node*> newBuckets(newBucketCount, nullptr);
        for (Node* head : buckets_) {
            Node* cur = head;
            while (cur) {
                Node* nxt = cur->next;
                std::size_t i = hash_(cur->key) % newBucketCount;
                cur->next = newBuckets[i];
                newBuckets[i] = cur;
                cur = nxt;
            }
        }
        buckets_.swap(newBuckets);
    }

public:
    explicit HashTable(std::size_t bucketCount = 16)
        : buckets_(bucketCount ? bucketCount : 16, nullptr) {}

    ~HashTable() { clear(); }

    // Non-copyable: raw owning pointers (same rule as the other structures).
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool insert(const K& key, const V& value) {
        std::size_t i = bucketIndex(key);
        for (Node* cur = buckets_[i]; cur; cur = cur->next) {
            if (cur->key == key) { cur->value = value; return false; }
        }
        Node* n = new Node(key, value);
        n->next = buckets_[i];
        buckets_[i] = n;
        ++size_;
        if (loadFactor() > MAX_LOAD_FACTOR) rehash(buckets_.size() * 2);
        return true;
    }

    bool get(const K& key, V& out) const {
        for (Node* cur = buckets_[bucketIndex(key)]; cur; cur = cur->next) {
            if (cur->key == key) { out = cur->value; return true; }
        }
        return false;
    }

    bool contains(const K& key) const {
        V ignored{};
        return get(key, ignored);
    }

    bool erase(const K& key) {
        std::size_t i = bucketIndex(key);
        Node* cur = buckets_[i];
        Node* prev = nullptr;
        while (cur && !(cur->key == key)) { prev = cur; cur = cur->next; }
        if (!cur) return false;
        if (prev) prev->next = cur->next;
        else buckets_[i] = cur->next;
        delete cur;
        --size_;
        return true;
    }

    void clear() {
        for (Node* head : buckets_) {
            while (head) { Node* tmp = head; head = head->next; delete tmp; }
        }
        for (Node*& head : buckets_) head = nullptr;
        size_ = 0;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    std::size_t bucketCount() const { return buckets_.size(); }
    double loadFactor() const {
        return buckets_.empty() ? 0.0
                                : static_cast<double>(size_) / buckets_.size();
    }

    std::string toString() const {
        std::ostringstream out;
        out << "{";
        bool first = true;
        for (const Node* head : buckets_) {
            for (const Node* cur = head; cur; cur = cur->next) {
                if (!first) out << ", ";
                first = false;
                out << cur->key << ": " << cur->value;
            }
        }
        out << "}";
        return out.str();
    }
};
