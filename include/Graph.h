#pragma once
#include <cstddef>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

// Directed graph stored as an adjacency list, implemented from scratch
// (C++17). Vertices are kept in a std::map so iteration (and therefore
// traversal order) is deterministic. Edges are directed by default;
// addUndirectedEdge adds both directions. BFS returns vertices in
// breadth-first visit order, DFS in depth-first (preorder) visit order;
// both return an empty vector when the start vertex is not in the graph.
// Vertices are created on demand by addEdge, so addVertex is only
// needed for isolated vertices.
template <typename T>
class Graph {
    std::map<T, std::vector<T>> adjacency_;
    std::size_t edgeCount_ = 0;

public:
    bool addVertex(const T& v) {
        if (adjacency_.count(v)) return false;
        adjacency_[v] = {};
        return true;
    }

    bool hasVertex(const T& v) const { return adjacency_.count(v) != 0; }

    // Adds a directed edge from -> to, creating missing vertices.
    // Returns false (and adds nothing) when the edge already exists.
    bool addEdge(const T& from, const T& to) {
        if (!hasVertex(from)) addVertex(from);
        if (!hasVertex(to)) addVertex(to);
        for (const T& n : adjacency_[from]) {
            if (n == to) return false;
        }
        adjacency_[from].push_back(to);
        ++edgeCount_;
        return true;
    }

    // Adds both directions; true when at least one of them was new.
    bool addUndirectedEdge(const T& a, const T& b) {
        bool first = addEdge(a, b);
        bool second = addEdge(b, a);
        return first || second;
    }

    bool hasEdge(const T& from, const T& to) const {
        auto it = adjacency_.find(from);
        if (it == adjacency_.end()) return false;
        for (const T& n : it->second) {
            if (n == to) return true;
        }
        return false;
    }

    bool removeEdge(const T& from, const T& to) {
        auto it = adjacency_.find(from);
        if (it == adjacency_.end()) return false;
        std::vector<T>& ns = it->second;
        for (std::size_t i = 0; i < ns.size(); ++i) {
            if (ns[i] == to) {
                ns.erase(ns.begin() + static_cast<std::ptrdiff_t>(i));
                --edgeCount_;
                return true;
            }
        }
        return false;
    }

    std::vector<T> neighbours(const T& v) const {
        auto it = adjacency_.find(v);
        return it == adjacency_.end() ? std::vector<T>{} : it->second;
    }

    std::vector<T> vertices() const {
        std::vector<T> out;
        for (const auto& pair : adjacency_) out.push_back(pair.first);
        return out;
    }

    std::vector<T> bfs(const T& start) const {
        std::vector<T> order;
        if (!hasVertex(start)) return order;
        std::set<T> visited;
        std::queue<T> q;
        visited.insert(start);
        q.push(start);
        while (!q.empty()) {
            T cur = q.front();
            q.pop();
            order.push_back(cur);
            for (const T& n : adjacency_.at(cur)) {
                if (!visited.count(n)) { visited.insert(n); q.push(n); }
            }
        }
        return order;
    }

    // Iterative DFS with an explicit stack; neighbours are pushed in
    // reverse so the visit order matches the recursive preorder.
    std::vector<T> dfs(const T& start) const {
        std::vector<T> order;
        if (!hasVertex(start)) return order;
        std::set<T> visited;
        std::stack<T> st;
        visited.insert(start);
        st.push(start);
        while (!st.empty()) {
            T cur = st.top();
            st.pop();
            order.push_back(cur);
            const std::vector<T>& ns = adjacency_.at(cur);
            for (auto it = ns.rbegin(); it != ns.rend(); ++it) {
                if (!visited.count(*it)) { visited.insert(*it); st.push(*it); }
            }
        }
        return order;
    }

    std::size_t vertexCount() const { return adjacency_.size(); }
    std::size_t edgeCount() const { return edgeCount_; }

    std::string toString() const {
        std::ostringstream out;
        out << "{";
        bool first = true;
        for (const auto& pair : adjacency_) {
            if (!first) out << ", ";
            first = false;
            out << pair.first << ": [";
            for (std::size_t i = 0; i < pair.second.size(); ++i) {
                if (i) out << ", ";
                out << pair.second[i];
            }
            out << "]";
        }
        out << "}";
        return out.str();
    }
};
