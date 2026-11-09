#pragma once
#include <cstddef>
#include <string>
#include <vector>

// Self-balancing binary search tree (AVL) implemented from scratch (C++17).
// Insertions and deletions rebalance with single/double rotations, so the
// height stays O(log n) even for sorted input. Operations: insert, search,
// erase, inorder / preorder / postorder traversals, height, size, and a
// balance check. Duplicates are ignored: insert returns false if present.
template <typename T>
class AVLTree {
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;
        explicit Node(const T& d) : data(d), left(nullptr), right(nullptr), height(0) {}
    };
    Node* root_ = nullptr;
    std::size_t size_ = 0;

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    int nodeHeight(Node* node) const { return node ? node->height : -1; }

    int balanceFactor(Node* node) const {
        return nodeHeight(node->left) - nodeHeight(node->right);
    }

    void updateHeight(Node* node) {
        int l = nodeHeight(node->left), r = nodeHeight(node->right);
        node->height = 1 + (l > r ? l : r);
    }

    // Right rotation: y's left child x becomes the new subtree root.
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    // Left rotation: y's right child x becomes the new subtree root.
    Node* rotateLeft(Node* y) {
        Node* x = y->right;
        y->right = x->left;
        x->left = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    // Restore the AVL invariant (|balance| <= 1) at this node.
    Node* rebalance(Node* node) {
        updateHeight(node);
        int balance = balanceFactor(node);
        if (balance > 1) {
            if (balanceFactor(node->left) < 0) node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1) {
            if (balanceFactor(node->right) > 0) node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    Node* insert(Node* node, const T& value, bool& inserted) {
        if (!node) { inserted = true; return new Node(value); }
        if (value < node->data) node->left = insert(node->left, value, inserted);
        else if (node->data < value) node->right = insert(node->right, value, inserted);
        else { inserted = false; return node; }
        return rebalance(node);
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
        return rebalance(node);
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

    bool balanced(Node* node) const {
        if (!node) return true;
        int balance = balanceFactor(node);
        return balance >= -1 && balance <= 1 && balanced(node->left) && balanced(node->right);
    }

public:
    ~AVLTree() { clear(); }

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
    int height() const { return nodeHeight(root_); }

    // True when every node satisfies the AVL invariant.
    bool isBalanced() const { return balanced(root_); }

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
