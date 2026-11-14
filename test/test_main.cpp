// dsa-cpp — assertion-based tests for every structure.
//
// Unlike src/main.cpp (a printed demo), this file checks behaviour with
// assertions and fails loudly: any CHECK that fails is reported with the
// section and line, and the process exits non-zero. Run it with ./test.sh
// (g++ -std=c++17, no dependencies).
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../include/SinglyLinkedList.h"
#include "../include/DoublyLinkedList.h"
#include "../include/ArrayStack.h"
#include "../include/CircularQueue.h"
#include "../include/BinarySearchTree.h"
#include "../include/AVLTree.h"
#include "../include/Heap.h"
#include "../include/HashTable.h"
#include "../include/Graph.h"

namespace {

int g_checks = 0;
int g_failures = 0;
const char* g_section = "";

#define SECTION(name) do { g_section = name; } while (0)
#define CHECK(cond) do { \
    ++g_checks; \
    if (!(cond)) { \
        ++g_failures; \
        std::cout << "FAIL [" << g_section << "] line " << __LINE__ \
                  << ": " << #cond << "\n"; \
    } \
} while (0)

template <typename T>
bool vecEq(const std::vector<T>& a, const std::vector<T>& b) {
    return a == b;
}

// ---------------------------------------------------------------- lists

void testSinglyLinkedList() {
    SECTION("SinglyLinkedList");
    SinglyLinkedList<int> list;
    CHECK(list.empty());
    CHECK(list.size() == 0);

    list.pushBack(10); list.pushBack(20); list.pushBack(30);
    list.pushFront(5);
    CHECK(list.toString() == "[5 -> 10 -> 20 -> 30]");
    CHECK(list.size() == 4);
    CHECK(list.contains(20));
    CHECK(!list.contains(99));

    CHECK(list.erase(20));
    CHECK(list.toString() == "[5 -> 10 -> 30]");
    CHECK(!list.erase(99));                    // missing: no change
    CHECK(list.size() == 3);

    list.reverse();
    CHECK(list.toString() == "[30 -> 10 -> 5]");

    CHECK(list.erase(30));                     // erase head
    CHECK(list.erase(5));                      // erase tail
    CHECK(list.toString() == "[10]");
    CHECK(list.erase(10));
    CHECK(list.empty());

    list.pushBack(1); list.pushBack(2);
    list.clear();
    CHECK(list.empty());
    CHECK(list.size() == 0);
    CHECK(!list.contains(1));
}

void testDoublyLinkedList() {
    SECTION("DoublyLinkedList");
    DoublyLinkedList<int> list;
    list.pushBack(10); list.pushBack(30);
    list.pushFront(5);
    list.insert(2, 20);                        // [5 <-> 10 <-> 20 <-> 30]
    CHECK(list.toString() == "[5 <-> 10 <-> 20 <-> 30]");
    CHECK(list.toStringReverse() == "[30 <-> 20 <-> 10 <-> 5]");
    CHECK(list.size() == 4);

    bool threw = false;
    try { list.insert(99, 1); } catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);

    list.insert(0, 1);
    CHECK(list.toString() == "[1 <-> 5 <-> 10 <-> 20 <-> 30]");
    list.insert(5, 40);
    CHECK(list.toString() == "[1 <-> 5 <-> 10 <-> 20 <-> 30 <-> 40]");

    CHECK(list.erase(20));
    CHECK(list.toString() == "[1 <-> 5 <-> 10 <-> 30 <-> 40]");
    CHECK(!list.erase(20));
    CHECK(list.contains(30));
    CHECK(!list.contains(20));

    list.reverse();
    CHECK(list.toString() == "[40 <-> 30 <-> 10 <-> 5 <-> 1]");
    CHECK(list.toStringReverse() == "[1 <-> 5 <-> 10 <-> 30 <-> 40]");

    list.clear();
    CHECK(list.empty());
    CHECK(list.toString() == "[]");
}

// ---------------------------------------------------------------- stack/queue

void testArrayStack() {
    SECTION("ArrayStack");
    ArrayStack<int> stack;
    CHECK(stack.empty());
    stack.push(1); stack.push(2); stack.push(3);
    CHECK(stack.size() == 3);
    CHECK(stack.peek() == 3);
    CHECK(stack.pop() == 3);
    CHECK(stack.pop() == 2);
    CHECK(stack.peek() == 1);
    CHECK(stack.pop() == 1);
    CHECK(stack.empty());

    bool threw = false;
    try { stack.pop(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    threw = false;
    try { stack.peek(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
}

void testCircularQueue() {
    SECTION("CircularQueue");
    CircularQueue<int> queue(4);
    CHECK(queue.empty());
    queue.enqueue(7); queue.enqueue(8); queue.enqueue(9);
    CHECK(queue.peek() == 7);
    CHECK(queue.dequeue() == 7);
    CHECK(queue.peek() == 8);
    CHECK(queue.size() == 2);

    // Grow past the initial capacity and wrap around: order must hold.
    CircularQueue<int> big(2);
    for (int i = 1; i <= 10; ++i) big.enqueue(i);
    CHECK(big.size() == 10);
    for (int i = 1; i <= 10; ++i) CHECK(big.dequeue() == i);
    CHECK(big.empty());

    bool threw = false;
    try { big.dequeue(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    threw = false;
    try { big.peek(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
}

// ---------------------------------------------------------------- trees

void testBinarySearchTree() {
    SECTION("BinarySearchTree");
    BinarySearchTree<int> bst;
    for (int v : {50, 30, 70, 20, 40, 60, 80}) CHECK(bst.insert(v));
    CHECK(!bst.insert(30));                    // duplicate rejected
    CHECK(bst.size() == 7);
    CHECK(bst.contains(40));
    CHECK(!bst.contains(99));
    CHECK(bst.height() == 2);

    CHECK(vecEq(bst.inorder(), std::vector<int>{20, 30, 40, 50, 60, 70, 80}));
    CHECK(vecEq(bst.preorder(), std::vector<int>{50, 30, 20, 40, 70, 60, 80}));
    CHECK(vecEq(bst.postorder(), std::vector<int>{20, 40, 30, 60, 80, 70, 50}));
    CHECK(bst.toString() == "[20, 30, 40, 50, 60, 70, 80]");

    CHECK(bst.erase(20));                      // leaf
    CHECK(bst.erase(30));                      // one child (40)
    CHECK(bst.erase(50));                      // two children (root)
    CHECK(!bst.erase(99));
    CHECK(vecEq(bst.inorder(), std::vector<int>{40, 60, 70, 80}));
    CHECK(bst.size() == 4);

    bst.clear();
    CHECK(bst.empty());
    CHECK(bst.height() == -1);
    CHECK(bst.inorder().empty());
}

void testAVLTree() {
    SECTION("AVLTree");
    AVLTree<int> avl;
    for (int v : {1, 2, 3, 4, 5, 6, 7}) CHECK(avl.insert(v));
    CHECK(!avl.insert(3));                     // duplicate rejected
    CHECK(avl.size() == 7);
    // Sorted input would make a plain BST 6 deep; the AVL stays shallow.
    CHECK(avl.isBalanced());
    CHECK(avl.height() <= 2);
    CHECK(vecEq(avl.inorder(), std::vector<int>{1, 2, 3, 4, 5, 6, 7}));
    CHECK(avl.contains(4));
    CHECK(!avl.contains(9));

    avl.erase(4);                              // root, two children
    CHECK(avl.isBalanced());
    CHECK(vecEq(avl.inorder(), std::vector<int>{1, 2, 3, 5, 6, 7}));
    avl.erase(1);
    avl.erase(7);
    CHECK(avl.isBalanced());
    CHECK(avl.height() <= 2);
    CHECK(avl.size() == 4);
    CHECK(!avl.erase(99));

    // A bigger sorted run still stays logarithmic.
    AVLTree<int> big;
    for (int v = 1; v <= 100; ++v) big.insert(v);
    CHECK(big.isBalanced());
    CHECK(big.height() <= 9);                  // log2(100) ~ 7, plus margin
    CHECK(big.size() == 100);
}

// ---------------------------------------------------------------- heaps

void testHeap() {
    SECTION("Heap");
    MaxHeap<int> maxHeap;
    for (int v : {15, 10, 20, 8, 25, 5}) maxHeap.push(v);
    CHECK(maxHeap.isHeap());
    CHECK(maxHeap.top() == 25);
    CHECK(maxHeap.size() == 6);
    std::vector<int> desc;
    while (!maxHeap.empty()) desc.push_back(maxHeap.pop());
    CHECK(vecEq(desc, std::vector<int>{25, 20, 15, 10, 8, 5}));

    MinHeap<int> minHeap(std::vector<int>{15, 10, 20, 8, 25, 5});
    CHECK(minHeap.isHeap());
    CHECK(minHeap.top() == 5);
    std::vector<int> asc;
    while (!minHeap.empty()) asc.push_back(minHeap.pop());
    CHECK(vecEq(asc, std::vector<int>{5, 8, 10, 15, 20, 25}));

    bool threw = false;
    try { minHeap.pop(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);
    threw = false;
    try { minHeap.top(); } catch (const std::runtime_error&) { threw = true; }
    CHECK(threw);

    CHECK(vecEq(Heap<int>::heapSort({9, 3, 7, 1, 8, 2, 6, 4, 5}),
                std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}));
    CHECK(Heap<int>::heapSort({}).empty());
    CHECK(vecEq(Heap<int>::heapSort({4, 2, 4, 1, 2}),
                std::vector<int>{1, 2, 2, 4, 4}));
    CHECK(vecEq(Heap<int>::heapSort({42}), std::vector<int>{42}));
}

// ---------------------------------------------------------------- hash table

void testHashTable() {
    SECTION("HashTable");
    HashTable<std::string, int> scores(4);     // tiny: forces collisions
    CHECK(scores.insert("alice", 10));
    CHECK(scores.insert("bob", 20));
    CHECK(scores.insert("carol", 30));
    CHECK(scores.insert("dave", 40));
    CHECK(scores.insert("erin", 50));
    CHECK(scores.size() == 5);
    CHECK(scores.bucketCount() == 8);          // rehashed at load factor 0.75

    int v = 0;
    CHECK(scores.get("bob", v) && v == 20);
    CHECK(!scores.get("mallory", v));
    CHECK(scores.contains("dave"));
    CHECK(scores.contains("carol"));
    CHECK(!scores.insert("alice", 99));        // duplicate: value updated
    CHECK(scores.get("alice", v) && v == 99);
    CHECK(scores.size() == 5);

    CHECK(scores.erase("carol"));
    CHECK(!scores.erase("mallory"));
    CHECK(!scores.contains("carol"));
    CHECK(scores.size() == 4);
    CHECK(scores.get("erin", v) && v == 50);   // survivors intact after erase

    HashTable<int, std::string> byId;
    CHECK(byId.insert(7, "seven"));
    CHECK(byId.insert(42, "forty-two"));
    std::string s;
    CHECK(byId.get(42, s) && s == "forty-two");
    byId.clear();
    CHECK(byId.empty());
    CHECK(byId.size() == 0);
}

// ---------------------------------------------------------------- graph

void testGraph() {
    SECTION("Graph");
    Graph<int> graph;
    for (int v : {1, 2, 3, 4, 5, 6}) CHECK(graph.addVertex(v));
    CHECK(!graph.addVertex(1));                // duplicate vertex
    CHECK(graph.addEdge(1, 2)); CHECK(graph.addEdge(1, 3));
    CHECK(graph.addEdge(2, 4)); CHECK(graph.addEdge(3, 4));
    CHECK(graph.addEdge(3, 5)); CHECK(graph.addEdge(4, 6));
    CHECK(graph.addEdge(5, 6));
    CHECK(!graph.addEdge(1, 2));              // duplicate edge
    CHECK(graph.vertexCount() == 6);
    CHECK(graph.edgeCount() == 7);
    CHECK(graph.hasEdge(1, 3));
    CHECK(!graph.hasEdge(3, 1));              // directed
    CHECK(vecEq(graph.neighbours(1), std::vector<int>{2, 3}));

    CHECK(vecEq(graph.bfs(1), std::vector<int>{1, 2, 3, 4, 5, 6}));
    CHECK(vecEq(graph.dfs(1), std::vector<int>{1, 2, 4, 6, 3, 5}));
    CHECK(graph.bfs(99).empty());              // missing start
    CHECK(graph.dfs(99).empty());

    CHECK(graph.removeEdge(3, 5));
    CHECK(!graph.hasEdge(3, 5));
    CHECK(graph.edgeCount() == 6);
    CHECK(!graph.removeEdge(3, 5));
    CHECK(vecEq(graph.bfs(1), std::vector<int>{1, 2, 3, 4, 6}));

    Graph<std::string> roads;
    CHECK(roads.addUndirectedEdge("A", "B"));
    CHECK(roads.addUndirectedEdge("B", "C"));
    CHECK(roads.hasEdge("A", "B"));
    CHECK(roads.hasEdge("B", "A"));
    CHECK(vecEq(roads.dfs("A"), std::vector<std::string>{"A", "B", "C"}));
    CHECK(roads.edgeWeight("A", "B") == 1.0);  // unweighted edge
    CHECK(roads.edgeWeight("A", "C") ==
          std::numeric_limits<double>::infinity());
}

void testDijkstra() {
    SECTION("Dijkstra");
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
    map.addVertex("Q");                        // isolated
    CHECK(!map.addWeightedEdge("A", "Z", -1)); // negative weight rejected
    CHECK(!map.addWeightedEdge("A", "B", 9));  // duplicate edge rejected
    CHECK(map.edgeWeight("A", "B") == 4);      // weight unchanged

    const std::map<std::string, double> dist = map.dijkstra("A");
    CHECK(dist.at("A") == 0);
    CHECK(dist.at("B") == 4);
    CHECK(dist.at("C") == 2);
    CHECK(dist.at("D") == 9);
    CHECK(dist.at("E") == 11);
    CHECK(dist.at("Z") == 14);
    CHECK(dist.at("Q") == std::numeric_limits<double>::infinity());

    // Cheapest A -> Z is A-B-D-E-Z = 14 (the B->C edge is one-way).
    CHECK(vecEq(map.shortestPath("A", "Z"),
                std::vector<std::string>{"A", "B", "D", "E", "Z"}));
    CHECK(vecEq(map.shortestPath("A", "A"), std::vector<std::string>{"A"}));
    CHECK(map.shortestPath("A", "Q").empty()); // unreachable
    CHECK(map.dijkstra("nope").empty());       // missing start

    map.removeEdge("D", "E");
    CHECK(map.dijkstra("A").at("Z") == 15);    // next best route costs 15
    CHECK(map.shortestPath("A", "Z").size() == 4);
}

} // namespace

int main() {
    testSinglyLinkedList();
    testDoublyLinkedList();
    testArrayStack();
    testCircularQueue();
    testBinarySearchTree();
    testAVLTree();
    testHeap();
    testHashTable();
    testGraph();
    testDijkstra();

    std::cout << "test_main: " << g_checks << " checks, "
              << g_failures << " failures\n";
    if (g_failures == 0) std::cout << "All tests passed\n";
    return g_failures == 0 ? 0 : 1;
}
