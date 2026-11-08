#pragma once
#include <cstddef>
#include <string>
#include <vector>

// Binary search tree implemented from scratch (C++17).
// Operations: insert, search, erase (leaf / one child / two children),
// inorder / preorder / postorder traversals, height, size.
// Duplicates are ignored: insert returns false if the value exists.
template <typename T>
class BinarySearchTree {
    struct Node {
        T data;
        Node* left;
        Node* right;
        explicit Node(const T& d) : data(d), left(nullptr), right(nullptr) {}
    };
    Node* root_ = nullptr;
    std::size_t size_ = 0;

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    Node* insert(Node* node, const T& value, bool& inserted) {
        if (!node) { inserted = true; return new Node(value); }
        if (value < node->data) node->left = insert(node->left, value, inserted);
        else if (node->data < value) node->right = insert(node->right, value, inserted);
        else inserted = false;
        return node;
    }

    Node* minNode(Node* node) const {
        while (node->left) node = node->left;
        return node;
    }

    Node* erase(Node* node, const T& value, bool& erased) {
        if (!node) { erased = false; return nullptr; }
        if (value < node->data) node->left = erase(node->left, value, erased);
        else if (node->data < value) node->right = erase(node->right, value, erased);
        else {
            erased = true;
            if (!node->left) { Node* tmp = node->right; delete node; return tmp; }
            if (!node->right) { Node* tmp = node->left; delete node; return tmp; }
            Node* succ = minNode(node->right);
            node->data = succ->data;
            node->right = erase(node->right, succ->data, erased);
            erased = true;  // successor removal must not clear the flag
        }
        return node;
    }

    void inorder(Node* node, std::vector<T>& out) const {
        if (!node) return;
        inorder(node->left, out);
        out.push_back(node->data);
        inorder(node->right, out);
    }

    void preorder(Node* node, std::vector<T>& out) const {
        if (!node) return;
        out.push_back(node->data);
        preorder(node->left, out);
        preorder(node->right, out);
    }

    void postorder(Node* node, std::vector<T>& out) const {
        if (!node) return;
        postorder(node->left, out);
        postorder(node->right, out);
        out.push_back(node->data);
    }

    int height(Node* node) const {
        if (!node) return -1;
        int l = height(node->left), r = height(node->right);
        return 1 + (l > r ? l : r);
    }

public:
    ~BinarySearchTree() { clear(); }

    bool insert(const T& value) {
        bool inserted = false;
        root_ = insert(root_, value, inserted);
        if (inserted) ++size_;
        return inserted;
    }

    bool contains(const T& value) const {
        Node* cur = root_;
        while (cur) {
            if (value < cur->data) cur = cur->left;
            else if (cur->data < value) cur = cur->right;
            else return true;
        }
        return false;
    }

    bool erase(const T& value) {
        bool erased = false;
        root_ = erase(root_, value, erased);
        if (erased) --size_;
        return erased;
    }

    void clear() { clear(root_); root_ = nullptr; size_ = 0; }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // Height in edges: -1 for an empty tree, 0 for a single node.
    int height() const { return height(root_); }

    std::vector<T> inorder() const { std::vector<T> out; inorder(root_, out); return out; }
    std::vector<T> preorder() const { std::vector<T> out; preorder(root_, out); return out; }
    std::vector<T> postorder() const { std::vector<T> out; postorder(root_, out); return out; }

    std::string toString() const {
        std::string out = "[";
        const std::vector<T> v = inorder();
        for (std::size_t i = 0; i < v.size(); ++i) {
            out += std::to_string(v[i]);
            if (i + 1 < v.size()) out += ", ";
        }
        return out + "]";
    }
};
