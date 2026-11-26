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
// needed for isolated vertices. topologicalSort() (Kahn's algorithm)
// returns a valid dependency order, or an empty vector when the graph
// has a cycle; isCyclic() answers the same question directly (DFS,
// three-colour marking), including self-loops. reverse() returns the
// transpose: every edge flipped, weights preserved, original untouched.
// connectedComponents() groups vertices joined by an edge in either
// direction (weakly connected components), ignoring edge direction.
//
// Weighted edges (addWeightedEdge) carry a non-negative double weight
// for Dijkstra's algorithm. Edges added with plain addEdge count as
// weight 1 in Dijkstra, so dijkstra() works on any graph; edgeWeight()
// returns the stored weight, 1 for an unweighted edge, and infinity
// when there is no edge at all. Dijkstra returns infinity for vertices
// unreachable from the start, and shortestPath() returns an empty
// vector when no path exists; shortestPathLength() gives just the
// cheapest distance (infinity when unreachable) without building
// the route.
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

    // Removes both directions of an undirected edge; true when at
    // least one of them existed. Removing only one direction would
    // leave a stray one-way edge, so callers of addUndirectedEdge
    // should remove with this instead of removeEdge. Works for
    // weighted undirected edges too: removeEdge drops the stored
    // weight of each direction it removes.
    bool removeUndirectedEdge(const T& a, const T& b) {
        bool first = removeEdge(a, b);
        bool second = removeEdge(b, a);
        return first || second;
    }

    // Removes a vertex and every edge touching it, outgoing and
    // incoming, including any stored weights for those edges.
    // Returns false when the vertex is not in the graph.
    bool removeVertex(const T& v) {
        auto it = adjacency_.find(v);
        if (it == adjacency_.end()) return false;
        edgeCount_ -= it->second.size();  // v's outgoing edges
        adjacency_.erase(it);
        for (auto& pair : adjacency_) {
            std::vector<T>& ns = pair.second;
            for (std::size_t i = 0; i < ns.size(); ++i) {
                if (ns[i] == v) {
                    ns.erase(ns.begin() + static_cast<std::ptrdiff_t>(i));
                    weights_.erase({pair.first, v});
                    --edgeCount_;  // an incoming edge to v
                    break;  // at most one edge per (from, to) pair
                }
            }
        }
        // Drop weights of v's own outgoing edges (a self-loop's weight
        // is covered here too, since v is already gone from adjacency_).
        for (auto wit = weights_.begin(); wit != weights_.end();) {
            if (wit->first.first == v) wit = weights_.erase(wit);
            else ++wit;
        }
        return true;
    }

    // Removes every vertex, edge and stored weight, leaving an empty
    // graph that can be built up again with the same instance.
    void clear() {
        adjacency_.clear();
        weights_.clear();
        edgeCount_ = 0;
    }

    std::vector<T> neighbours(const T& v) const {
        auto it = adjacency_.find(v);
        return it == adjacency_.end() ? std::vector<T>{} : it->second;
    }

    // Out-degree: number of edges leaving v (0 when v is missing).
    std::size_t outDegree(const T& v) const {
        auto it = adjacency_.find(v);
        return it == adjacency_.end() ? 0 : it->second.size();
    }

    // In-degree: number of edges arriving at v (0 when v is missing).
    // A self-loop counts once in each degree.
    std::size_t inDegree(const T& v) const {
        if (!hasVertex(v)) return 0;
        std::size_t count = 0;
        for (const auto& pair : adjacency_) {
            for (const T& n : pair.second) {
                if (n == v) ++count;
            }
        }
        return count;
    }

    // Transpose: a new graph with every vertex kept and every edge
    // flipped (to -> from). Weighted edges keep their weight in the
    // reversed direction; unweighted edges stay unweighted. The
    // original graph is untouched, so reverse() twice returns a graph
    // equal to the original (same edges, insertion order aside).
    Graph<T> reverse() const {
        Graph<T> out;
        for (const T& v : vertices()) out.addVertex(v);
        for (const auto& pair : adjacency_) {
            for (const T& n : pair.second) {
                auto it = weights_.find({pair.first, n});
                if (it != weights_.end()) out.addWeightedEdge(n, pair.first, it->second);
                else out.addEdge(n, pair.first);
            }
        }
        return out;
    }

    std::vector<T> vertices() const {
        std::vector<T> out;
        for (const auto& pair : adjacency_) out.push_back(pair.first);
        return out;
    }

    // Every directed edge as (from, to) pairs, ordered by source
    // vertex (vertices are stored sorted) then by insertion order
    // within a vertex. An undirected edge appears as both directions,
    // and a self-loop appears once, as (v, v).
    std::vector<std::pair<T, T>> edges() const {
        std::vector<std::pair<T, T>> out;
        for (const auto& pair : adjacency_) {
            for (const T& n : pair.second) out.emplace_back(pair.first, n);
        }
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

    // Reachability: true when a directed path exists from start to
    // goal (a vertex always reaches itself). False when either vertex
    // is missing. A BFS from start that stops as soon as the goal is
    // reached, so it answers the question without building the full
    // traversal.
    bool hasPath(const T& start, const T& goal) const {
        if (!hasVertex(start) || !hasVertex(goal)) return false;
        if (start == goal) return true;
        std::set<T> visited;
        std::queue<T> q;
        visited.insert(start);
        q.push(start);
        while (!q.empty()) {
            T cur = q.front();
            q.pop();
            for (const T& n : adjacency_.at(cur)) {
                if (n == goal) return true;
                if (!visited.count(n)) { visited.insert(n); q.push(n); }
            }
        }
        return false;
    }

    // Topological order (Kahn's algorithm): every vertex appears
    // after all of its predecessors, so the order is a valid build /
    // dependency order for the graph. Returns an empty vector when
    // the graph has a cycle (no valid order exists) or is empty.
    // Deterministic: zero in-degree vertices are taken in sorted
    // vertex order, and neighbours are processed in insertion order.
    std::vector<T> topologicalSort() const {
        std::map<T, std::size_t> indegree;
        for (const T& v : vertices()) indegree[v] = 0;
        for (const auto& pair : adjacency_) {
            for (const T& n : pair.second) ++indegree[n];
        }
        std::queue<T> q;
        for (const auto& pair : indegree) {
            if (pair.second == 0) q.push(pair.first);
        }
        std::vector<T> order;
        while (!q.empty()) {
            T cur = q.front();
            q.pop();
            order.push_back(cur);
            for (const T& n : adjacency_.at(cur)) {
                if (--indegree[n] == 0) q.push(n);
            }
        }
        // A leftover vertex means a cycle kept its in-degree above 0.
        if (order.size() != adjacency_.size()) return {};
        return order;
    }

    // Cycle check (DFS, three-colour marking): true when any directed
    // cycle exists, including a self-loop; false for an empty graph
    // or a DAG. Complements topologicalSort(), which reports a cycle
    // only as an empty order.
    bool isCyclic() const {
        std::set<T> visiting;  // on the current DFS path (grey)
        std::set<T> done;      // fully explored (black)
        for (const T& v : vertices()) {
            if (!done.count(v) && hasCycleFrom(v, visiting, done)) return true;
        }
        return false;
    }

    // Weakly connected components: groups of vertices joined by an
    // edge in either direction, so edge direction is ignored — a
    // vertex that only points at a group still belongs to it. Each
    // component lists its vertices in BFS order, and components come
    // out ordered by their smallest vertex (the smallest unvisited
    // vertex starts each one). An empty graph has no components; an
    // isolated vertex is a component of one.
    std::vector<std::vector<T>> connectedComponents() const {
        // Keep the vertex list in a named variable: calling
        // vertices().begin() and vertices().end() would build two
        // separate temporaries, and the range would span two buffers.
        std::vector<T> all = vertices();
        std::set<T> unvisited(all.begin(), all.end());
        std::vector<std::vector<T>> components;
        while (!unvisited.empty()) {
            T start = *unvisited.begin();
            std::vector<T> component;
            std::queue<T> q;
            unvisited.erase(start);
            q.push(start);
            while (!q.empty()) {
                T cur = q.front();
                q.pop();
                component.push_back(cur);
                // Outgoing neighbours, plus every vertex pointing at
                // cur: the undirected view of the graph.
                for (const T& n : adjacency_.at(cur)) {
                    if (unvisited.erase(n)) q.push(n);
                }
                for (const auto& pair : adjacency_) {
                    for (const T& n : pair.second) {
                        if (n == cur && unvisited.erase(pair.first)) {
                            q.push(pair.first);
                        }
                    }
                }
            }
            components.push_back(component);
        }
        return components;
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

    // The cost of the cheapest route from start to goal, without
    // building the route itself: 0 when start == goal, infinity when
    // the goal is unreachable or either vertex is missing. Cheaper
    // than shortestPath() when only the distance is needed.
    double shortestPathLength(const T& start, const T& goal) const {
        if (!hasVertex(start) || !hasVertex(goal)) {
            return std::numeric_limits<double>::infinity();
        }
        return dijkstraImpl(start).first[goal];
    }

    std::size_t vertexCount() const { return adjacency_.size(); }
    std::size_t edgeCount() const { return edgeCount_; }

private:
    // Recursive step of isCyclic(): a back edge to a vertex still on
    // the current DFS path means a cycle; a self-loop is such an edge.
    bool hasCycleFrom(const T& v, std::set<T>& visiting, std::set<T>& done) const {
        visiting.insert(v);
        for (const T& n : adjacency_.at(v)) {
            if (visiting.count(n)) return true;
            if (!done.count(n) && hasCycleFrom(n, visiting, done)) return true;
        }
        visiting.erase(v);
        done.insert(v);
        return false;
    }

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
