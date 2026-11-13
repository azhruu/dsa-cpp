#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include "../include/SinglyLinkedList.h"
#include "../include/DoublyLinkedList.h"
#include "../include/ArrayStack.h"
#include "../include/CircularQueue.h"
#include "../include/BinarySearchTree.h"
#include "../include/AVLTree.h"
#include "../include/Heap.h"
#include "../include/HashTable.h"
#include "../include/Graph.h"
#include <vector>

template <typename T>
static std::string join(const std::vector<T>& v) {
    std::ostringstream out;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) out << " ";
        out << v[i];
    }
    return out.str();
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

    std::cout << "\n=== Graph (adjacency list) ===\n";
    Graph<int> graph;
    // Edges: 1->2, 1->3, 2->4, 3->4, 3->5, 4->6, 5->6
    for (int v : {1, 2, 3, 4, 5, 6}) graph.addVertex(v);
    graph.addEdge(1, 2); graph.addEdge(1, 3);
    graph.addEdge(2, 4); graph.addEdge(3, 4); graph.addEdge(3, 5);
    graph.addEdge(4, 6); graph.addEdge(5, 6);
    std::cout << "graph:      " << graph.toString() << "\n";
    std::cout << "BFS from 1: " << join(graph.bfs(1)) << "\n";
    std::cout << "DFS from 1: " << join(graph.dfs(1)) << "\n";
    std::cout << "vertices=" << graph.vertexCount() << ", edges=" << graph.edgeCount()
              << ", hasEdge(1,3)=" << graph.hasEdge(1, 3)
              << ", hasEdge(3,1)=" << graph.hasEdge(3, 1) << "\n";
    std::cout << "addEdge dup 1->2 -> " << graph.addEdge(1, 2) << "\n";
    graph.removeEdge(3, 5);
    std::cout << "removeEdge(3,5): " << graph.toString() << "\n";
    std::cout << "BFS from 99 (missing): '" << join(graph.bfs(99)) << "' (empty)\n";
    Graph<std::string> roads;
    roads.addUndirectedEdge("A", "B");
    roads.addUndirectedEdge("B", "C");
    std::cout << "undirected: " << roads.toString()
              << ", DFS from A: " << join(roads.dfs("A")) << "\n";
    std::cout << "unweighted edge weight: " << roads.edgeWeight("A", "B")
              << ", missing edge weight: " << roads.edgeWeight("A", "C") << "\n";

    std::cout << "\n=== Dijkstra shortest path ===\n";
    // Classic weighted example: A-B 4, A-C 2, B-C 1, B-D 5, C-D 8,
    // C-E 10, D-E 2, D-Z 6, E-Z 3. Cheapest A->Z is A-B-D-E-Z = 14.
    Graph<std::string> map;
    map.addWeightedEdge("A", "B", 4);
    map.addWeightedEdge("A", "C", 2);
    map.addWeightedEdge("B", "C", 1);
    map.addWeightedEdge("B", "D", 5);
    map.addWeightedEdge("C", "D", 8);
    map.addWeightedEdge("C", "E", 10);
    map.addWeightedEdge("D", "E", 2);
    map.addWeightedEdge("D", "Z", 6);
    map.addWeightedEdge("E", "Z", 3);
    map.addVertex("Q");  // isolated: unreachable from A
    auto fmtDist = [](double d) {
        if (d == std::numeric_limits<double>::infinity()) return std::string("inf");
        if (d == std::floor(d)) return std::to_string(static_cast<long long>(d));
        return std::to_string(d);
    };
    std::map<std::string, double> dist = map.dijkstra("A");
    std::cout << "distances from A: ";
    for (const std::string& v : map.vertices()) std::cout << v << "=" << fmtDist(dist[v]) << " ";
    std::cout << "\n";
    std::cout << "shortest path A -> Z: " << join(map.shortestPath("A", "Z")) << " (cost 14)\n";
    std::cout << "shortest path A -> A: " << join(map.shortestPath("A", "A")) << "\n";
    std::cout << "shortest path A -> Q (unreachable): '" << join(map.shortestPath("A", "Q")) << "' (empty)\n";
    std::cout << "dijkstra from missing vertex: " << map.dijkstra("nope").size() << " entries (empty)\n";
    std::cout << "negative weight rejected: addWeightedEdge(A, Z, -1) -> "
              << map.addWeightedEdge("A", "Z", -1) << "\n";
    std::cout << "duplicate weighted edge rejected: addWeightedEdge(A, B, 9) -> "
              << map.addWeightedEdge("A", "B", 9)
              << ", weight still " << map.edgeWeight("A", "B") << "\n";
    map.removeEdge("D", "E");
    std::cout << "after removeEdge(D, E): shortest path A -> Z: "
              << join(map.shortestPath("A", "Z")) << " (cost 15)\n";

    std::cout << "\nAll structures working OK\n";
    return 0;
}
