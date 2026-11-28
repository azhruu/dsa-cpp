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
    CHECK(graph.outDegree(1) == 2);
    CHECK(graph.inDegree(4) == 2);           // edges 2->4 and 3->4
    CHECK(graph.inDegree(1) == 0);           // nothing points at the root
    CHECK(graph.outDegree(99) == 0);         // missing vertex
    CHECK(graph.inDegree(99) == 0);
    CHECK(vecEq(graph.inNeighbours(4), std::vector<int>{2, 3}));  // edges 2->4 and 3->4
    CHECK(graph.inNeighbours(1).empty());      // nothing points at the root
    CHECK(graph.inNeighbours(99).empty());     // missing vertex
    CHECK(vecEq(graph.edges(), std::vector<std::pair<int, int>>{
        {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}}));

    CHECK(vecEq(graph.bfs(1), std::vector<int>{1, 2, 3, 4, 5, 6}));
    CHECK(vecEq(graph.dfs(1), std::vector<int>{1, 2, 4, 6, 3, 5}));
    CHECK(graph.bfs(99).empty());              // missing start
    CHECK(graph.dfs(99).empty());

    CHECK(graph.hasPath(1, 6));            // 1->2->4->6 and 1->3->5->6
    CHECK(graph.hasPath(1, 5));
    CHECK(!graph.hasPath(6, 1));           // directed: no way back
    CHECK(graph.hasPath(3, 3));            // a vertex reaches itself
    CHECK(!graph.hasPath(1, 99));          // missing goal
    CHECK(!graph.hasPath(99, 1));          // missing start

    // Topological order: every vertex comes after its predecessors.
    CHECK(vecEq(graph.topologicalSort(), std::vector<int>{1, 2, 3, 4, 5, 6}));
    Graph<int> cyclic;
    cyclic.addEdge(1, 2); cyclic.addEdge(2, 3); cyclic.addEdge(3, 1);
    CHECK(cyclic.topologicalSort().empty());  // a cycle has no valid order
    Graph<int> emptyGraph;
    CHECK(emptyGraph.topologicalSort().empty());

    // Cycle check: the DAG is acyclic, the 1->2->3->1 graph is not,
    // and neither is an empty graph. A self-loop is a cycle too.
    CHECK(!graph.isCyclic());
    CHECK(cyclic.isCyclic());
    CHECK(!emptyGraph.isCyclic());
    Graph<int> selfLoop;
    selfLoop.addEdge(1, 1);
    CHECK(selfLoop.isCyclic());

    // connectedComponents(): weakly connected groups — edge direction
    // is ignored, so a vertex that only points at a group still
    // belongs to it. Components are ordered by their smallest vertex;
    // an isolated vertex is a component of one, an empty graph has none.
    Graph<int> islands;
    islands.addEdge(1, 2); islands.addEdge(2, 3);
    islands.addEdge(4, 5); islands.addVertex(6);  // isolated
    islands.addEdge(8, 7);  // one-way edge: 8 still joins 7's group
    CHECK(vecEq(islands.connectedComponents(), std::vector<std::vector<int>>{
        {1, 2, 3}, {4, 5}, {6}, {7, 8}}));
    CHECK(graph.connectedComponents().size() == 1);  // main graph is one piece
    CHECK(emptyGraph.connectedComponents().empty());
    Graph<int> oneWay;
    oneWay.addEdge(1, 2); oneWay.addEdge(3, 1);  // 3 can reach 1, not vice versa
    CHECK(oneWay.connectedComponents().size() == 1);  // direction ignored

    // isolatedVertices(): no edges in or out. Only 6 qualifies here:
    // 7 and 8 are joined by an edge, and every other vertex has one.
    CHECK(vecEq(islands.isolatedVertices(), std::vector<int>{6}));
    CHECK(graph.isolatedVertices().empty());       // every vertex is joined
    CHECK(emptyGraph.isolatedVertices().empty());
    Graph<int> selfLoopOnly;
    selfLoopOnly.addVertex(1); selfLoopOnly.addVertex(2);
    selfLoopOnly.addEdge(1, 1);  // a self-loop is still an edge
    CHECK(vecEq(selfLoopOnly.isolatedVertices(), std::vector<int>{2}));
    Graph<int> targetOnly;
    targetOnly.addEdge(1, 2); targetOnly.addVertex(3);
    CHECK(vecEq(targetOnly.isolatedVertices(), std::vector<int>{3}));
    // 2 is only ever a target, so it is not isolated either.

    // reverse() (transpose): every edge flips direction, vertices and
    // weights are kept, and the original graph is untouched.
    Graph<int> reversed = graph.reverse();
    CHECK(vecEq(reversed.edges(), std::vector<std::pair<int, int>>{
        {2, 1}, {3, 1}, {4, 2}, {4, 3}, {5, 3}, {6, 4}, {6, 5}}));
    CHECK(reversed.vertexCount() == 6);
    CHECK(reversed.edgeCount() == 7);
    CHECK(reversed.hasEdge(2, 1));
    CHECK(!reversed.hasEdge(1, 2));           // direction really flipped
    CHECK(vecEq(graph.edges(), std::vector<std::pair<int, int>>{
        {1, 2}, {1, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 6}, {5, 6}}));
    CHECK(emptyGraph.reverse().vertexCount() == 0);  // empty stays empty
    Graph<std::string> weightedRevSrc;
    weightedRevSrc.addWeightedEdge("A", "B", 4);
    weightedRevSrc.addWeightedEdge("B", "C", 1);
    weightedRevSrc.addEdge("C", "A");          // unweighted edge
    Graph<std::string> weightedRev = weightedRevSrc.reverse();
    CHECK(weightedRev.hasEdge("B", "A"));
    CHECK(weightedRev.edgeWeight("B", "A") == 4);    // weight kept
    CHECK(weightedRev.edgeWeight("C", "B") == 1);
    CHECK(weightedRev.edgeWeight("A", "C") == 1.0);  // unweighted stays 1
    CHECK(!weightedRev.hasEdge("A", "B"));
    CHECK(weightedRev.edgeCount() == 3);

    CHECK(graph.removeEdge(3, 5));
    CHECK(!graph.hasEdge(3, 5));
    CHECK(graph.edgeCount() == 6);
    CHECK(!graph.removeEdge(3, 5));
    CHECK(vecEq(graph.edges(), std::vector<std::pair<int, int>>{
        {1, 2}, {1, 3}, {2, 4}, {3, 4}, {4, 6}, {5, 6}}));  // 3->5 gone
    CHECK(vecEq(graph.bfs(1), std::vector<int>{1, 2, 3, 4, 6}));

    CHECK(graph.removeVertex(5));          // takes its outgoing 5->6 with it
    CHECK(!graph.hasVertex(5));
    CHECK(!graph.hasEdge(5, 6));
    CHECK(graph.vertexCount() == 5);
    CHECK(graph.edgeCount() == 5);
    CHECK(vecEq(graph.bfs(1), std::vector<int>{1, 2, 3, 4, 6}));
    CHECK(graph.removeVertex(4));          // also drops incoming 2->4, 3->4
    CHECK(!graph.hasEdge(2, 4));
    CHECK(!graph.hasEdge(3, 4));
    CHECK(graph.vertexCount() == 4);
    CHECK(graph.edgeCount() == 2);
    CHECK(graph.inDegree(6) == 0);           // both routes into 6 are gone
    CHECK(!graph.removeVertex(99));        // missing vertex

    Graph<std::string> weighted;
    weighted.addWeightedEdge("A", "B", 4);
    weighted.addWeightedEdge("B", "C", 1);
    CHECK(weighted.removeVertex("B"));     // drops both edges and weights
    CHECK(weighted.edgeWeight("A", "B") ==
          std::numeric_limits<double>::infinity());
    CHECK(weighted.edgeWeight("B", "C") ==
          std::numeric_limits<double>::infinity());
    CHECK(weighted.edgeCount() == 0);

    Graph<std::string> roads;
    CHECK(roads.addUndirectedEdge("A", "B"));
    CHECK(roads.addUndirectedEdge("B", "C"));
    CHECK(roads.hasEdge("A", "B"));
    CHECK(roads.hasEdge("B", "A"));
    CHECK(vecEq(roads.dfs("A"), std::vector<std::string>{"A", "B", "C"}));
    CHECK(roads.edgeWeight("A", "B") == 1.0);  // unweighted edge
    CHECK(roads.edgeWeight("A", "C") ==
          std::numeric_limits<double>::infinity());
    CHECK(vecEq(roads.edges(), std::vector<std::pair<std::string, std::string>>{
        {"A", "B"}, {"B", "A"}, {"B", "C"}, {"C", "B"}}));  // both directions

    // removeUndirectedEdge drops both directions at once; removing
    // only one would leave a stray one-way edge behind.
    CHECK(roads.removeUndirectedEdge("B", "C"));
    CHECK(!roads.hasEdge("B", "C"));
    CHECK(!roads.hasEdge("C", "B"));
    CHECK(roads.edgeCount() == 2);
    CHECK(!roads.removeUndirectedEdge("B", "C"));  // already gone
    CHECK(!roads.removeUndirectedEdge("A", "Z"));   // never existed

    // clear() empties the graph — vertices, edges and weights — and
    // the same instance can be built up again afterwards.
    graph.clear();
    CHECK(graph.vertexCount() == 0);
    CHECK(graph.edgeCount() == 0);
    CHECK(graph.edges().empty());
    CHECK(!graph.hasVertex(1));
    CHECK(graph.bfs(1).empty());
    CHECK(graph.addEdge(7, 8));              // reuse after clear
    CHECK(graph.vertexCount() == 2);
    CHECK(graph.edgeCount() == 1);
    CHECK(graph.hasEdge(7, 8));
    CHECK(vecEq(graph.edges(), std::vector<std::pair<int, int>>{{7, 8}}));

    Graph<std::string> weightedClear;
    weightedClear.addWeightedEdge("A", "B", 4);
    weightedClear.clear();
    CHECK(weightedClear.vertexCount() == 0);
    CHECK(weightedClear.edgeCount() == 0);
    CHECK(weightedClear.edgeWeight("A", "B") ==
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
    CHECK(map.shortestPathLength("A", "Z") == 14);
    CHECK(map.shortestPathLength("A", "A") == 0);
    CHECK(map.shortestPathLength("A", "Q") ==
          std::numeric_limits<double>::infinity()); // unreachable
    CHECK(map.shortestPathLength("A", "nope") ==
          std::numeric_limits<double>::infinity()); // missing goal
    CHECK(map.shortestPathLength("nope", "Z") ==
          std::numeric_limits<double>::infinity()); // missing start

    map.removeEdge("D", "E");
    CHECK(map.dijkstra("A").at("Z") == 15);    // next best route costs 15
    CHECK(map.shortestPath("A", "Z").size() == 4);
    CHECK(map.shortestPathLength("A", "Z") == 15);
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
