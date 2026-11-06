# dsa-cpp

**Classic data structures, implemented from scratch in modern C++17** — header-only templates, RAII memory management, no STL containers doing the structures' own work. Companion to [dsa-java](https://github.com/azhruu/dsa-java), same syllabus in a second language.

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-blue?style=for-the-badge)
![Status](https://img.shields.io/badge/status-actively%20building-brightgreen?style=for-the-badge)

## Implemented

| Area | Structures |
|---|---|
| Lists | Singly Linked List (push/erase/find/reverse, destructor-cleaned) |
| Stacks | Array Stack (LIFO, exception on underflow) |
| Queues | Circular Queue (growable ring buffer) |

## Build & run

```bash
./build.sh        # g++ -std=c++17, then runs the demo
```

Requires any C++17 compiler (g++ / clang++).

## Coming next

Doubly Linked List → Binary Search Tree → AVL → Heaps → Hash Table → Graphs (BFS/DFS, Dijkstra). Full plan: [ROADMAP.md](ROADMAP.md).

## License

MIT — see [LICENSE](LICENSE).
