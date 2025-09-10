#include <iostream>
#include <cmath>
#include <vector>
#include <cstdint>

using Vertex = int;
using Weight = long long;

const Weight DIST_INF = std::numeric_limits<Weight>::max() / 4;

class DirectedGraph {
public:

    const static int MAX_GRAPH_SIZE = std::numeric_limits<int>::max() / 2;
    const static Weight MAX_EDGE_WEIGHT = std::numeric_limits<int>::max() / 2;

    struct Edge {
        Vertex adjacent_vertex;
        Weight weight;
    };

    using AdjacencyList = std::vector<Edge>;

protected:

    int n = 0; // number of vertices
    int m = 0; // number of edges

    std::vector<AdjacencyList> adjacency_lists; // vertex numbers from 1 to n

public:

    bool isCorrectVertex(Vertex u) const {
        return static_cast<Vertex>(1) <= u && u <= static_cast<Vertex>(n);
    }

    bool isCorrectWeight(Weight w) const {
        return std::abs(w) <= MAX_EDGE_WEIGHT;
    }

    bool isCorrectParameters() const {
        if (n == 0 && m != 0) {
            return false;
        }
        if (n < 0 || n > MAX_GRAPH_SIZE) {
            return false;
        }
        return true;
    }

public:

    void readGraph(std::istream& in = std::cin) {
        in >> n >> m;

        if (!isCorrectParameters()) {
            throw std::invalid_argument("Incorrect graph parameters!");
        }

        adjacency_lists.assign(n + 1, AdjacencyList{});

        for (int i = 0; i < m; ++i) {
            Vertex u, v;
            in >> u >> v;

            if (!isCorrectVertex(u) || !isCorrectVertex(v)) {
                throw std::invalid_argument("Incorrect vertex!");
            }

            Weight weight;
            in >> weight;

            if (!isCorrectWeight(weight)) {
                throw std::invalid_argument("Incorrect weight!");
            }

            adjacency_lists[u].push_back(Edge(v, weight));
        }
    }

    const AdjacencyList& getAdjacentVertices(Vertex u) const {
        if (!isCorrectVertex(u)) {
            throw std::runtime_error("Incorrect vertex!");
        }
        return adjacency_lists[u];
    }

    int getCountOfVertices() const {
        return n;
    }

    void clear() {
        n = 0;
        m = 0;

        adjacency_lists.clear();
        adjacency_lists.shrink_to_fit();
    }
};

std::pair<std::vector<Weight>, std::vector<Vertex>> dijkstraWithMarks(const DirectedGraph& g, const Vertex start_vertex) {
    int n = g.getCountOfVertices();
    
    if (!g.isCorrectVertex(start_vertex)) {
        throw std::invalid_argument("Incorrect start vertex!");
    }

    std::vector<Weight> dist(n + 1, DIST_INF);
    std::vector<Vertex> from(n + 1, 0);
    
    dist[start_vertex] = 0;

    std::vector<bool> visited(n + 1, false);

    for (int i = 0; i < n; ++i) {

        Vertex nearest = 1;
        while (visited[nearest]) {
            ++nearest;
        }

        for (Vertex vertex = nearest + 1; vertex <= n; ++vertex) {
            if (visited[vertex]) {
                continue;
            }
            if (dist[vertex] < dist[nearest]) {
                nearest = vertex;
            }
        }

        visited[nearest] = true;

        if (dist[nearest] == DIST_INF) {
            continue;
        }

        for (const auto &[adjacent_vertex, weight]: g.getAdjacentVertices(nearest)) {
            if (dist[adjacent_vertex] > dist[nearest] + weight) {
                dist[adjacent_vertex] = dist[nearest] + weight;
                from[adjacent_vertex] = nearest;
            }
        }
    }
    
    return {dist, from};
}

std::pair<std::vector<Weight>, std::vector<Vertex>> fordBellman(const DirectedGraph& g, const Vertex start_vertex) {
    int n = g.getCountOfVertices();
    
    if (!g.isCorrectVertex(start_vertex)) {
        throw std::invalid_argument("Incorrect start vertex!");
    }

    std::vector<Weight> dist(n + 1, DIST_INF);
    std::vector<Vertex> from(n + 1, 0);
    
    dist[start_vertex] = 0;

    for (int i = 0; i + 1 != n; ++i) {
        for (Vertex u = 1; u <= n; ++u) {
            for (const auto &[v, weight]: g.getAdjacentVertices(u)) {
                if (dist[u] != DIST_INF && dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    from[v] = u;
                }
            }
        }
    }
    
    return {dist, from};
}


int main() {

    freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

    DirectedGraph g;
    g.readGraph();

    for (long long V: fordBellman(g, 1).first) {
        std::cout << V << " ";
    }

    return 0;
}
