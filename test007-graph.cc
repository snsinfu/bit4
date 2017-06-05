#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Eigen/Dense>

struct Node;

struct Edge
{
    Node* end = nullptr;
    double weight = 0.0;
};

struct Node
{
    int id = 0;
    std::vector<Edge> connections;
};

struct Graph
{
    std::vector<Node> nodes;
};

std::ostream& operator<<(std::ostream& os, Graph const& graph)
{
    os << "graph {\n";
    for (Node const& node : graph.nodes) {
        os << " " << node.id << ";";
    }
    os << "\n";
    for (Node const& node : graph.nodes) {
        os << " " << node.id << " -- {";
        for (Edge const& edge : node.connections) {
            if (edge.end->id > node.id) {
                os << " " << edge.end->id;
            }
        }
        os << " }\n";
    }
    os << "}";
    return os;
}

template<typename T, typename Dist, typename Engine>
auto generate_different_value(T const& value, Dist&& dist, Engine& engine)
{
    typename std::decay_t<Dist>::result_type result;
    do {
        result = dist(engine);
    } while (result == value);
    return result;
}

template<typename RNG>
Eigen::MatrixXi generate_adjacency_matrix(int n, RNG& engine)
{
    // Random walk over nodes until all nodes are visited.
    auto adj = Eigen::MatrixXi(n, n).setZero();
    auto dist = std::uniform_int_distribution<int>{0, n - 1};
    auto node = dist(engine);
    auto visited = std::unordered_set<int>{node};
    while (visited.size() != static_cast<std::size_t>(n)) {
        auto const next = generate_different_value(node, dist, engine);
        adj(node, next) = 1;
        node = next;
        visited.insert(node);
    }
    return adj;
}

Graph make_graph()
{
    Graph graph;
    int const num_nodes = 10;

    for (int i = 0; i < num_nodes; ++i) {
        Node node;
        node.id = i;
        graph.nodes.push_back(node);
    }

    std::mt19937 engine;

    auto adj = generate_adjacency_matrix(num_nodes, engine);
    adj += adj.transpose().eval();

    auto weight_dist = std::uniform_real_distribution<double>{0.1, 1.0};
    for (auto& node : graph.nodes) {
        for (auto pair = 0; pair < num_nodes; ++pair) {
            if (adj(node.id, pair) != 0) {
                Edge edge;
                edge.end = &graph.nodes[static_cast<std::size_t>(pair)];
                edge.weight = weight_dist(engine);
                node.connections.push_back(edge);
            }
        }
    }

    return graph;
}

int main()
{
    Graph graph = make_graph();
    std::cout << graph << "\n";
}
