#include <iostream>
#include "../include/SinglyLinkedList.h"
#include "../include/DoublyLinkedList.h"
#include "../include/ArrayStack.h"
#include "../include/CircularQueue.h"

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

    std::cout << "\nAll structures working OK\n";
    return 0;
}
