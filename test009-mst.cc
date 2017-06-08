#include <deque>
#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <type_traits>
#include <vector>

#include <Eigen/Dense>

template<typename Dist, typename RNG, typename T>
auto generate_excluding(Dist&& dist, RNG& engine, T const& value)
{
    typename std::decay_t<Dist>::result_type result;
    do {
        result = dist(engine);
    } while (result == value);
    return result;
}

/*
 * generate_graph generates a random connected graph of n vertices. Indices of
 * connected vertices are passed to the function conn.
 */
template<typename RNG, typename Function>
void generate_graph(int n, RNG& engine, Function conn)
{
    // Random walk over nodes until all nodes are visited.
    auto index = std::uniform_int_distribution<int>{0, n - 1};
    auto current = index(engine);
    auto visited = std::vector<bool>(n);
    auto n_visited = 0;
    for (;;) {
        if (!visited[current]) {
            visited[current] = true;
            n_visited++;
            if (n_visited == n) {
                break;
            }
        }
        auto const next = generate_excluding(index, engine, current);
        conn(current, next);
        current = next;
    }
}

template<typename Function>
void compute_mst(Eigen::MatrixXd const& graph, Function conn)
{
    auto const n = graph.rows();

    struct Connection {
        int start;
        int end;
        double cost;
    };
    auto const dummy = Connection{
        -1, -1, std::numeric_limits<double>::infinity()
    };
    auto cheapest = std::vector<Connection>(n, dummy);

    auto nodes = std::set<int>{};
    for (int i = 0; i < n; ++i) {
        nodes.insert(i);
    }
    while (!nodes.empty()) {
        auto choice = *nodes.begin();
        for (auto i : nodes) {
            if (cheapest[i].cost < cheapest[choice].cost) {
                choice = i;
            }
        }
        nodes.erase(choice);

        if (cheapest[choice].end != -1) {
            conn(choice, cheapest[choice].end);
        }

        for (auto i : nodes) {
            if (graph(choice, i) != 0 && graph(choice, i) < cheapest[i].cost) {
                cheapest[i].start = i;
                cheapest[i].end = choice;
                cheapest[i].cost = graph(choice, i);
            }
        }
    }
}

int main()
{
    auto engine = std::mt19937_64{};
    auto weight_dist = std::uniform_int_distribution<int>{1, 99};
    auto graph = Eigen::MatrixXd{10, 10}.setZero();
    generate_graph(graph.rows(), engine, [&](int i, int j) {
        graph(i, j) = graph(j, i) = static_cast<double>(weight_dist(engine));
    });

    auto tree = Eigen::MatrixXd{10, 10}.setZero();
    compute_mst(graph, [&](int i, int j) {
        tree(i, j) = tree(j, i) = graph(i, j);
    });

    std::cout << tree << '\n';
}
