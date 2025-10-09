#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

using idx_t = int;
using Weight = long long;

const Weight DIST_INF = 2e18;

const idx_t MAX_GRAPH_SIZE = 1e9;
const Weight MAX_EDGE_WEIGHT = 1e9;

class DirectedGraph {
public:

    struct Edge {
        idx_t adjacent_vertex;
        Weight weight;
    };

    using AdjacencyList = std::vector<Edge>;

protected:

    idx_t n = 0; // number of vertices
    idx_t m = 0; // number of edges

    std::vector<AdjacencyList> adjacency_lists; // vertex numbers from 1 to n

public:

    bool isCorrectVertex(idx_t u) const {
        return 1 <= u && u <= n;
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

        for (idx_t i = 0; i < m; ++i) {
            idx_t u, v;
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

    const AdjacencyList& getAdjacentVertices(const idx_t u) const {
        if (!isCorrectVertex(u)) {
            throw std::runtime_error("Incorrect vertex!");
        }
        return adjacency_lists[u];
    }

    idx_t getCountOfVertices() const {
        return n;
    }

    idx_t getCountOfEdges() const {
        return m;
    }

    void clear() {
        n = m = 0;
        adjacency_lists.clear();
        adjacency_lists.shrink_to_fit();
    }
};

struct FullEdge {
    idx_t u, v;
    Weight weight;
};

std::pair<std::vector<Weight>, std::vector<idx_t>> dijkstraWithMarks(const DirectedGraph& g, const idx_t start_vertex) {

    /*
     * Naive Dijkstra's algorithm (with marks)
     *
     * Finds all the shortest paths from the starting
     * vertex to the others (or INF if there is no path) and
     * paths (previous vertices or -1 if there is no path to)
     * 
     * g - adjacency lists - g[Ui] = {{Vij, Wij}, ...}
     *
     * | Multiple Edges |   Loops   |  D/UD |  u   |  w   |
	 * +----------------+-----------+-------+------+------+
	 * |      V         |     V     | V / V | >= 1 | >= 0 |
     * 
     * Time Complexity:   O(N^2)
     * Memory Complexity: O(N)
    */

    const idx_t n = g.getCountOfVertices();
    
    if (!g.isCorrectVertex(start_vertex)) {
        throw std::invalid_argument("Incorrect start vertex!");
    }

    std::vector<Weight> dist(n + 1, DIST_INF);
    std::vector<idx_t> path(n + 1, -1);
    
    dist[start_vertex] = 0;

    std::vector<bool> visited(n + 1, false);

    for (idx_t i = 0; i < n; ++i) {

        idx_t nearest = 1;
        while (visited[nearest]) {
            ++nearest;
        }

        for (idx_t vertex = nearest + 1; vertex <= n; ++vertex) {
            if (!visited[vertex] && dist[vertex] < dist[nearest]) {
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
                path[adjacent_vertex] = nearest;
            }
        }
    }
    
    return std::make_pair(dist, path);
}

std::pair<std::vector<Weight>, std::vector<idx_t>> fordBellman(const DirectedGraph& g, const idx_t start_vertex) {

    /*
    * Ford-Bellman algorithm
    *
    * Finds all the shortest distances from the starting
    * vertex to the others (or INF if there is no path, or -INF
    * if there is no shortest path [negative cycle]) and paths
    * (previous vertices or -1 if there is no path to)
    * 
    * g - adjacency lists - g[Ui] = {{Vij, Wij}, ...} 
    *
    * | Multiple Edges |   Loops   |  D/UD |  u   |  w  |
    * +----------------+-----------+-------+------+-----+
    * |      V         |     V     | V / V | >= 0 | any |
    * 
    * Time Complexity:   O(N*M)
    * Memory Complexity: O(N + M)
    */

    const idx_t n = g.getCountOfVertices();
    
    if (!g.isCorrectVertex(start_vertex)) {
        throw std::invalid_argument("Incorrect start vertex!");
    }

    std::vector<FullEdge> edges;
    edges.reserve(g.getCountOfEdges());

    for (idx_t u = 1; u <= n; ++u) {
        for (const auto &[v, weight]: g.getAdjacentVertices(u))  {
            edges.push_back(FullEdge(u, v, weight));
        }
    }
    
    std::vector<Weight> dist(n + 1, DIST_INF);
    std::vector<idx_t> path(n + 1, -1);
    
    dist[start_vertex] = 0;

    for (idx_t i = 0; i + 1 < n; ++i) {
        for (const auto& [u, v, weight]: edges) {
			if (dist[u] != DIST_INF && dist[v] > dist[u] + weight) {
				dist[v] = std::max(dist[u] + weight, -DIST_INF);
				path[v] = u;
			}
        }
    }
    
    for (idx_t i = 0; i + 1 < n; ++i) {
        for (const auto& [u, v, weight]: edges) {
			if (dist[u] != DIST_INF && dist[v] > dist[u] + weight) {
				dist[v] = -DIST_INF;
				path[v] = -1;
			}
        }
    }

    return std::make_pair(dist, path);
}


int main() {
    try {
        std::freopen("input.txt", "r", stdin);

        DirectedGraph g;
        g.readGraph();

        const idx_t start_vertex = 1;

        // --- Dijkstra ---
        {
            const auto [dist, path] = dijkstraWithMarks(g, start_vertex);

            std::ofstream out("dijkstra.txt");
            if (!out.is_open()) {
                throw std::runtime_error("Cannot open dijkstra.txt for writing");
            }

            for (idx_t i = 1; i <= g.getCountOfVertices(); ++i) {
                if (dist[i] >= DIST_INF)
                    out << "INF ";
                else
                    out << dist[i] << " ";
            }
            out << "\n";

            for (idx_t i = 1; i <= g.getCountOfVertices(); ++i) {
                out << path[i] << " ";
            }
            out << "\n";

            out.close();
        }

        // --- Ford–Bellman ---
        {
            const auto [dist, path] = fordBellman(g, start_vertex);

            std::ofstream out("fordbellman.txt");
            if (!out.is_open()) {
                throw std::runtime_error("Cannot open fordbellman.txt for writing");
            }

            for (idx_t i = 1; i <= g.getCountOfVertices(); ++i) {
                if (dist[i] >= DIST_INF) {
                    out << "INF ";
                }
                else if (dist[i] <= -DIST_INF) {
                    out << "-INF ";
                }
                else {
                    out << dist[i] << " ";
                }
            }
            out << "\n";

            for (idx_t i = 1; i <= g.getCountOfVertices(); ++i) {
                out << path[i] << " ";
            }
            out << "\n";

            out.close();
        }

    } 
    catch (const std::exception &e) {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}