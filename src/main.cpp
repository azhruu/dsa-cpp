#include <iostream>
#include "../include/SinglyLinkedList.h"
#include "../include/DoublyLinkedList.h"
#include "../include/ArrayStack.h"
#include "../include/CircularQueue.h"
#include "../include/BinarySearchTree.h"
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

    std::cout << "\nAll structures working OK\n";
    return 0;
}
