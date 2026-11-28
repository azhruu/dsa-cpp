# dsa-cpp

**Classic data structures, implemented from scratch in modern C++17** — header-only templates, RAII memory management, no STL containers doing the structures' own work. Companion to [dsa-java](https://github.com/azhruu/dsa-java), same syllabus in a second language.

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-blue?style=for-the-badge)
![Status](https://img.shields.io/badge/status-actively%20building-brightgreen?style=for-the-badge)

## Implemented

| Area | Structures |
|---|---|
| Lists | Singly Linked List (push/erase/find/reverse, destructor-cleaned) |
| Lists | Doubly Linked List (bidirectional, insert at index, forward/backward traversal) |
| Stacks | Array Stack (LIFO, exception on underflow) |
| Queues | Circular Queue (growable ring buffer) |
| Trees | Binary Search Tree (insert/search/erase, inorder/preorder/postorder) |
| Trees | AVL Tree (self-balancing via rotations, stays O(log n) on sorted input) |
| Heaps | Max/Min Heap (array-backed, push/pop in O(log n), O(n) heapify) + heap sort |
| Hash Tables | Hash Table (separate chaining, rehash at load factor 0.75) |
| Graphs | Graph (adjacency list, BFS/DFS, directed + undirected edges, Dijkstra shortest path + path reconstruction, shortestPathLength() distance-only query, hasPath() reachability query, topologicalSort() dependency order, isCyclic() cycle check, reverse() transpose with weights preserved, connectedComponents() weakly connected groups, vertex/edge removal, removeUndirectedEdge() drops both directions of an undirected edge, in/out-degree queries, inNeighbours() predecessors query, isolatedVertices() listing vertices with no edges in or out, edges() listing every directed edge, clear() to empty and reuse) |

## Build & run

```bash
./build.sh        # g++ -std=c++17, then runs the demo
```

Or with CMake (same compiler flags, out-of-source build):

```bash
cmake -S . -B build
cmake --build build
./build/demo          # printed demo of every structure
ctest --test-dir build   # the 264-check test suite
```

Requires any C++17 compiler (g++ / clang++) and CMake 3.16+ for the CMake path.

## Tests

```bash
./test.sh         # compiles test/test_main.cpp and runs it
```

`test/test_main.cpp` is an assertion-based test main for every structure — 283 checks covering list/stack/queue operations (including edge cases like empty-pop exceptions and queue growth), BST/AVL traversals and balance, heap ordering and heap sort, hash-table collisions and rehashing, and graph BFS/DFS, in/out-degree queries, inNeighbours() predecessors, edge listing, reverse() transpose (including weight preservation), connectedComponents() weakly connected groups, undirected edge removal and vertex removal plus Dijkstra distances and path reconstruction. Any failure prints the section and line and exits non-zero. `src/main.cpp` remains the printed demo; the tests are the verification.

## Coming next

The roadmap is complete — every planned structure and the CMake build are done. Full plan: [ROADMAP.md](ROADMAP.md).

## License

MIT — see [LICENSE](LICENSE).
