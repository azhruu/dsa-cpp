#include <iostream>
#include "../include/SinglyLinkedList.h"
#include "../include/DoublyLinkedList.h"
#include "../include/ArrayStack.h"
#include "../include/CircularQueue.h"
#include "../include/BinarySearchTree.h"
#include "../include/AVLTree.h"
#include "../include/Heap.h"
#include "../include/HashTable.h"
#include <vector>

static std::string join(const std::vector<int>& v) {
    std::string out;
    for (std::size_t i = 0; i < v.size(); ++i) {
        out += std::to_string(v[i]);
        if (i + 1 < v.size()) out += " ";
    }
    return out;
}

int main() {
    std::cout << "=== SinglyLinkedList ===\n";
    SinglyLinkedList<int> list;
    list.pushBack(10); list.pushBack(20); list.pushBack(30);
    list.pushFront(5);
    std::cout << "list:       " << list.toString() << "\n";
    list.erase(20);
    std::cout << "erase(20):  " << list.toString() << "\n";
    list.reverse();
    std::cout << "reversed:   " << list.toString() << "\n";
    std::cout << "size=" << list.size() << ", contains(15)=" << list.contains(15) << "\n";

    std::cout << "\n=== DoublyLinkedList ===\n";
    DoublyLinkedList<int> dlist;
    dlist.pushBack(10); dlist.pushBack(30);
    dlist.pushFront(5);
    dlist.insert(2, 20);                    // [5 <-> 10 <-> 20 <-> 30]
    std::cout << "list:       " << dlist.toString() << "\n";
    std::cout << "backwards:  " << dlist.toStringReverse() << "\n";
    dlist.erase(20);
    std::cout << "erase(20):  " << dlist.toString() << "\n";
    dlist.reverse();
    std::cout << "reversed:   " << dlist.toString() << "\n";
    std::cout << "size=" << dlist.size() << ", contains(10)=" << dlist.contains(10) << "\n";

    std::cout << "\n=== ArrayStack ===\n";
    ArrayStack<int> stack;
    stack.push(1); stack.push(2); stack.push(3);
    std::cout << "pop -> " << stack.pop() << ", peek -> " << stack.peek() << "\n";

    std::cout << "\n=== CircularQueue ===\n";
    CircularQueue<int> queue(4);
    queue.enqueue(7); queue.enqueue(8); queue.enqueue(9);
    std::cout << "dequeue -> " << queue.dequeue() << ", peek -> " << queue.peek() << "\n";

    std::cout << "\n=== BinarySearchTree ===\n";
    BinarySearchTree<int> bst;
    for (int v : {50, 30, 70, 20, 40, 60, 80}) bst.insert(v);
    std::cout << "inorder:    " << join(bst.inorder()) << "\n";
    std::cout << "preorder:   " << join(bst.preorder()) << "\n";
    std::cout << "postorder:  " << join(bst.postorder()) << "\n";
    std::cout << "contains(40)=" << bst.contains(40) << ", contains(99)=" << bst.contains(99)
              << ", size=" << bst.size() << ", height=" << bst.height() << "\n";
    std::cout << "insert dup 30 -> " << bst.insert(30) << "\n";
    bst.erase(20);                                     // leaf
    std::cout << "erase leaf 20:      " << bst.toString() << "\n";
    bst.erase(30);                                     // one child left (40)
    std::cout << "erase one-child 30: " << bst.toString() << "\n";
    bst.erase(50);                                     // two children (root)
    std::cout << "erase two-child 50: " << bst.toString() << "\n";
    std::cout << "erase missing 99 -> " << bst.erase(99) << ", size=" << bst.size() << "\n";

    std::cout << "\n=== AVLTree ===\n";
    AVLTree<int> avl;
    // Sorted input would make a plain BST a 6-deep chain; the AVL
    // rebalances on every insert and stays shallow.
    for (int v : {1, 2, 3, 4, 5, 6, 7}) avl.insert(v);
    std::cout << "inorder:    " << join(avl.inorder()) << "\n";
    std::cout << "preorder:   " << join(avl.preorder()) << "\n";
    std::cout << "height=" << avl.height() << ", size=" << avl.size()
              << ", balanced=" << avl.isBalanced() << "\n";
    std::cout << "contains(4)=" << avl.contains(4) << ", contains(9)=" << avl.contains(9)
              << ", insert dup 3 -> " << avl.insert(3) << "\n";
    avl.erase(4);                                      // root, two children
    std::cout << "erase root 4: " << avl.toString() << ", height=" << avl.height()
              << ", balanced=" << avl.isBalanced() << "\n";
    avl.erase(1);                                      // leaf
    avl.erase(7);                                      // leaf
    std::cout << "erase 1, 7:   " << avl.toString() << ", height=" << avl.height()
              << ", balanced=" << avl.isBalanced() << ", size=" << avl.size() << "\n";
    std::cout << "erase missing 99 -> " << avl.erase(99) << "\n";

    std::cout << "\n=== MaxHeap / MinHeap ===\n";
    MaxHeap<int> maxHeap;
    for (int v : {15, 10, 20, 8, 25, 5}) maxHeap.push(v);
    std::cout << "max-heap:   " << maxHeap.toString() << ", top=" << maxHeap.top()
              << ", isHeap=" << maxHeap.isHeap() << "\n";
    std::cout << "pops (descending): ";
    while (!maxHeap.empty()) std::cout << maxHeap.pop() << " ";
    std::cout << "\n";
    MinHeap<int> minHeap(std::vector<int>{15, 10, 20, 8, 25, 5});  // heapified
    std::cout << "min-heap:   " << minHeap.toString() << ", top=" << minHeap.top()
              << ", isHeap=" << minHeap.isHeap() << "\n";
    std::cout << "pops (ascending):  ";
    while (!minHeap.empty()) std::cout << minHeap.pop() << " ";
    std::cout << "\n";
    try { minHeap.pop(); } catch (const std::exception& e) {
        std::cout << "pop on empty heap throws: " << e.what() << "\n";
    }

    std::cout << "\n=== HeapSort ===\n";
    std::cout << "sorted:     " << join(Heap<int>::heapSort({9, 3, 7, 1, 8, 2, 6, 4, 5})) << "\n";
    std::cout << "empty:      " << join(Heap<int>::heapSort({})) << "\n";
    std::cout << "duplicates: " << join(Heap<int>::heapSort({4, 2, 4, 1, 2})) << "\n";

    std::cout << "\n=== HashTable (separate chaining) ===\n";
    HashTable<std::string, int> scores(4);   // tiny on purpose: forces collisions
    scores.insert("alice", 10); scores.insert("bob", 20); scores.insert("carol", 30);
    scores.insert("dave", 40); scores.insert("erin", 50);
    std::cout << "table:      " << scores.toString() << "\n";
    std::cout << "size=" << scores.size() << ", buckets=" << scores.bucketCount()
              << ", load=" << scores.loadFactor() << "\n";
    int v = 0;
    std::cout << "get(bob)=" << (scores.get("bob", v) ? std::to_string(v) : "?")
              << ", get(mallory)=" << scores.get("mallory", v) << "\n";
    std::cout << "insert dup alice -> " << scores.insert("alice", 99)
              << " (value updated: ";
    scores.get("alice", v);
    std::cout << v << ")\n";
    std::cout << "erase(carol) -> " << scores.erase("carol")
              << ", erase(mallory) -> " << scores.erase("mallory") << "\n";
    std::cout << "contains(dave)=" << scores.contains("dave")
              << ", contains(carol)=" << scores.contains("carol")
              << ", size=" << scores.size() << "\n";
    std::cout << "after erase: " << scores.toString() << "\n";
    HashTable<int, std::string> byId;
    byId.insert(7, "seven"); byId.insert(42, "forty-two");
    std::string s;
    std::cout << "int keys:   " << byId.toString()
              << ", get(42)=" << (byId.get(42, s) ? s : "?") << "\n";

    std::cout << "\nAll structures working OK\n";
    return 0;
}
