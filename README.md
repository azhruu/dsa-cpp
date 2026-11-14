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
| Graphs | Graph (adjacency list, BFS/DFS, directed + undirected edges, Dijkstra shortest path + path reconstruction) |

## Build & run

```bash
./build.sh        # g++ -std=c++17, then runs the demo
```

Requires any C++17 compiler (g++ / clang++).

## Tests

```bash
./test.sh         # compiles test/test_main.cpp and runs it
```

`test/test_main.cpp` is an assertion-based test main for every structure — 200 checks covering list/stack/queue operations (including edge cases like empty-pop exceptions and queue growth), BST/AVL traversals and balance, heap ordering and heap sort, hash-table collisions and rehashing, and graph BFS/DFS plus Dijkstra distances and path reconstruction. Any failure prints the section and line and exits non-zero. `src/main.cpp` remains the printed demo; the tests are the verification.

## Coming next

CMake build alongside build.sh next. Full plan: [ROADMAP.md](ROADMAP.md).

## License

MIT — see [LICENSE](LICENSE).
