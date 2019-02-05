#include <cstdint>
#include <cstddef>
#include <utility>
#include <vector>


template<typename T>
class bucket_graph
{
public:
    using uint32_t = std::uint32_t;

    explicit bucket_graph(uint32_t bucket_count)
        : buckets_(bucket_count)
    {
    }

    void put(uint32_t i, T value)
    {
        buckets_[i].members.push_back(value);
    }

    void link(uint32_t i, uint32_t j)
    {
        buckets_[i].neighbors.push_back(j);
    }

    template<typename Callback>
    void foreach_neighbor(Callback callback) const
    {
        for (const auto& bucket : buckets_) {
            for (const auto neighbor_index : bucket.neighbors) {
                foreach_among(bucket, buckets_[neighbor_index], callback);
            }
        }
    }

private:
    struct bucket_type
    {
        std::vector<T> members;
        std::vector<uint32_t> neighbors;
    };

    template<typename Callback>
    void foreach_among(
        const bucket_type& bucket1,
        const bucket_type& bucket2,
        Callback callback
    ) const
    {
        for (const auto& m1 : bucket1.members) {
            for (const auto& m2 : bucket2.members) {
                callback(m1, m2);
            }
        }
    }

    std::vector<bucket_type> buckets_;
};


#include <iostream>
#include <string>


int main()
{
    bucket_graph<std::string> graph{3};

    graph.link(0, 1);
    graph.link(0, 2);
    graph.link(2, 1);

    graph.put(0, "abc");
    graph.put(0, "xyz");
    graph.put(1, "123");
    graph.put(1, "456");
    graph.put(2, ":-)");
    graph.put(2, "<3");

    graph.foreach_neighbor([&](const std::string& a, const std::string& b) {
        std::cout << a << '\t' << b << '\n';
    });
}
