#pragma once
#include <algorithm>
#include <cstddef>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

// Directed graph stored as an adjacency list, implemented from scratch
// (C++17). Vertices are kept in a std::map so iteration (and therefore
// traversal order) is deterministic. Edges are directed by default;
// addUndirectedEdge adds both directions. BFS returns vertices in
// breadth-first visit order, DFS in depth-first (preorder) visit order;
// both return an empty vector when the start vertex is not in the graph.
// Vertices are created on demand by addEdge, so addVertex is only
// needed for isolated vertices.
//
// Weighted edges (addWeightedEdge) carry a non-negative double weight
// for Dijkstra's algorithm. Edges added with plain addEdge count as
// weight 1 in Dijkstra, so dijkstra() works on any graph; edgeWeight()
// returns the stored weight, 1 for an unweighted edge, and infinity
// when there is no edge at all. Dijkstra returns infinity for vertices
// unreachable from the start, and shortestPath() returns an empty
// vector when no path exists.
template <typename T>
class Graph {
    std::map<T, std::vector<T>> adjacency_;
    std::map<std::pair<T, T>, double> weights_;
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

    // Adds a directed edge with a weight for Dijkstra. Negative
    // weights are rejected (Dijkstra needs non-negative weights) and
    // an edge that already exists keeps its old weight; both cases
    // return false and add nothing.
    bool addWeightedEdge(const T& from, const T& to, double weight) {
        if (weight < 0) return false;
        if (!addEdge(from, to)) return false;
        weights_[{from, to}] = weight;
        return true;
    }

    // Weighted version of addUndirectedEdge; true when at least one
    // direction was new.
    bool addWeightedUndirectedEdge(const T& a, const T& b, double weight) {
        bool first = addWeightedEdge(a, b, weight);
        bool second = addWeightedEdge(b, a, weight);
        return first || second;
    }

    // Weight of an edge: the stored weight, 1 for an unweighted edge,
    // infinity when there is no edge.
    double edgeWeight(const T& from, const T& to) const {
        auto it = weights_.find({from, to});
        if (it != weights_.end()) return it->second;
        return hasEdge(from, to) ? 1.0 : std::numeric_limits<double>::infinity();
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
                weights_.erase({from, to});
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

    // Dijkstra's shortest-path distances from start to every vertex:
    // start maps to 0, unreachable vertices map to infinity, and a
    // missing start returns an empty map. Runs in O((V + E) log V)
    // on the min-heap; unweighted edges count as weight 1.
    std::map<T, double> dijkstra(const T& start) const {
        return dijkstraImpl(start).first;
    }

    // The cheapest route from start to goal as a vertex list
    // (start ... goal), or an empty vector when goal is unreachable
    // or either vertex is missing. A start equal to the goal returns
    // just {start}.
    std::vector<T> shortestPath(const T& start, const T& goal) const {
        std::vector<T> path;
        if (!hasVertex(start) || !hasVertex(goal)) return path;
        auto [dist, prev] = dijkstraImpl(start);
        if (dist[goal] == std::numeric_limits<double>::infinity()) return path;
        for (T cur = goal;; cur = prev[cur]) {
            path.push_back(cur);
            if (cur == start) break;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::size_t vertexCount() const { return adjacency_.size(); }
    std::size_t edgeCount() const { return edgeCount_; }

private:
    // Shared core of dijkstra() and shortestPath(): distances plus,
    // for every reached vertex except start, the previous vertex on
    // its cheapest route. Empty maps when start is not in the graph.
    std::pair<std::map<T, double>, std::map<T, T>> dijkstraImpl(const T& start) const {
        std::map<T, double> dist;
        std::map<T, T> prev;
        if (!hasVertex(start)) return {dist, prev};
        const double INF = std::numeric_limits<double>::infinity();
        for (const T& v : vertices()) dist[v] = INF;
        dist[start] = 0;
        using Entry = std::pair<double, T>;  // (distance so far, vertex)
        std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
        pq.push({0, start});
        while (!pq.empty()) {
            auto [d, cur] = pq.top();
            pq.pop();
            if (d > dist[cur]) continue;  // stale heap entry
            for (const T& n : adjacency_.at(cur)) {
                double via = dist[cur] + edgeWeight(cur, n);
                if (via < dist[n]) {
                    dist[n] = via;
                    prev[n] = cur;
                    pq.push({via, n});
                }
            }
        }
        return {dist, prev};
    }

public:
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
