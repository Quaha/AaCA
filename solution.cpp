#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <random>

using idx_t = int;

namespace Graphs {

    using Weight = long long;

    const Weight DIST_INF = 2e18;

    const idx_t MAX_GRAPH_SIZE = 1e9;
    const Weight MAX_EDGE_WEIGHT = 1e9;

    struct Edge {
        idx_t u, v;
        Weight weight;
    };

    struct DirectedEdge {
        idx_t adjacent_vertex;
        Weight weight;
    };

    using AdjacencyList = std::vector<DirectedEdge>;
    using AdjacencyMatrix = std::vector<std::vector<Weight>>;
    using EdgeList = std::vector<Edge>;

    class DirectedGraph {
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

                adjacency_lists[u].push_back(DirectedEdge(v, weight));
            }
        }

        void printGraph(std::ostream& out = std::cout) const {
            out << n << " " << m << "\n";
            for (idx_t u = 1; u <= n; ++u) {
                for (const auto& [v, w] : getAdjacentVertices(u)) {
                    out << u << " " << v << " " << w << "\n";
                }
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

        void generateRandomGraph(const idx_t n, const idx_t m, const Weight q, const Weight r) {

            this->clear();

            this->n = n;
            this->m = m;

            if (!isCorrectParameters())
                throw std::invalid_argument("Incorrect graph parameters!");
            if (q > r)
                throw std::invalid_argument("Incorrect weight bounds!");
            if (n > MAX_GRAPH_SIZE)
                throw std::invalid_argument("Graph too large!");

            adjacency_lists.resize(n + 1);

            std::random_device rd;
            std::mt19937_64 rng(rd());
            std::uniform_int_distribution<idx_t> vertex_dist(1, n);
            std::uniform_int_distribution<Weight> weight_dist(q, r);

            for (idx_t i = 0; i < m; ++i) {
                idx_t u = vertex_dist(rng);
                idx_t v = vertex_dist(rng);
                Weight w = weight_dist(rng);

                adjacency_lists[u].push_back(DirectedEdge(v, w));
            }
        }

        AdjacencyMatrix getAdjacencyMatrix() const {
            AdjacencyMatrix matrix(n + 1, std::vector<Weight>(n + 1, DIST_INF));

            for (idx_t curr_V = 1; curr_V <= n; ++curr_V) {
                for (auto [next_V, w]: getAdjacentVertices(curr_V)) {
                    matrix[curr_V][next_V] = std::min(matrix[curr_V][next_V], w);
                }
            }

            return matrix;
        }

        EdgeList getEdgeList() const {
            EdgeList edges;

            for (idx_t curr_V = 1; curr_V <= n; ++curr_V) {
                for (auto [next_V, w]: getAdjacentVertices(curr_V)) {
                    edges.push_back({curr_V, next_V, w});
                }
            }       

            return edges;
        }
    };

    std::pair<std::vector<Weight>, std::vector<idx_t>> dijkstraWithMarks(const idx_t n, const AdjacencyMatrix& g, const idx_t start_vertex) {

        /*
        * Naive Dijkstra's algorithm (with marks)
        *
        * Finds all the shortest paths from the starting
        * vertex to the others (or INF if there is no path) and
        * paths (previous vertices or -1 if there is no path to)
        * 
        * g - adjacency matrix - g[u][v] = Wuv (or +INF)
        *
        * Time Complexity: O(N^2)
        */

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

            for (idx_t adjacent_vertex = 1; adjacent_vertex <= n; ++adjacent_vertex) {
                Weight weight = g[nearest][adjacent_vertex];
                if (dist[adjacent_vertex] > dist[nearest] + weight) {
                    dist[adjacent_vertex] = dist[nearest] + weight;
                    path[adjacent_vertex] = nearest;
                }
            }
        }
        
        return std::make_pair(dist, path);
    }

    std::pair<std::vector<Weight>, std::vector<idx_t>> fordBellman(const idx_t n, const EdgeList& g, const idx_t start_vertex) {

        /*
        * Ford-Bellman algorithm
        *
        * Finds all the shortest distances from the starting
        * vertex to the others (or +INF if there is no path) and
        * paths (previous vertices or -1 if there is no path to)
        * 
        * g - edge list - g = {{u1, v1, w1}, {u2, v2, w2}, ...} 
        * 
        * Time Complexity:   O(N*M)
        */

        std::vector<Weight> dist(n + 1, DIST_INF);
        std::vector<idx_t> path(n + 1, -1);
        
        dist[start_vertex] = 0;

        for (idx_t i = 0; i + 1 < n; ++i) {
            for (const auto& [u, v, weight]: g) {
                if (dist[u] != DIST_INF && dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    path[v] = u;
                }
            }
        }

        return std::make_pair(dist, path);
    }

} // namespace Graphs

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    try {
        std::freopen("input.txt", "r", stdin);

        Graphs::DirectedGraph g;
        g.readGraph();

        const idx_t start_vertex = 1;

        // --- Dijkstra ---
        {
            Graphs::AdjacencyMatrix matrix = g.getAdjacencyMatrix();
            const auto [dist, path] = Graphs::dijkstraWithMarks(g.getCountOfVertices(), matrix, start_vertex);

            std::ofstream out("dijkstra.txt");
            if (!out.is_open()) {
                throw std::runtime_error("Cannot open dijkstra.txt for writing");
            }

            for (idx_t i = 1; i <= g.getCountOfVertices(); ++i) {
                if (dist[i] >= Graphs::DIST_INF)
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

            Graphs::EdgeList edge_list = g.getEdgeList();
            const auto [dist, path] = fordBellman(g.getCountOfVertices(), edge_list, start_vertex);

            std::ofstream out("fordbellman.txt");
            if (!out.is_open()) {
                throw std::runtime_error("Cannot open fordbellman.txt for writing");
            }

            for (idx_t i = 1; i <= g.getCountOfVertices(); ++i) {
                if (dist[i] >= Graphs::DIST_INF) {
                    out << "INF ";
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