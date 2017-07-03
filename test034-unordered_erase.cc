#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include <cstddef>


template<typename T>
struct Bag {
    using iterator = typename std::vector<T>::iterator;

    std::size_t size() const
    {
        return storage_.size();
    }

    iterator begin()
    {
        return storage_.begin();
    }

    iterator end()
    {
        return storage_.end();
    }

    void insert(T const& value)
    {
        storage_.push_back(value);
    }

    void erase(iterator it)
    {
        *it = std::move(storage_.back());
        storage_.pop_back();
    }

  private:
    std::vector<T> storage_;
};


int main() {
    std::mt19937_64 engine;
    {
        std::random_device source;
        std::seed_seq seed{source(), source(), source(), source()};
        engine.seed(seed);
    }

    Bag<int> numbers;

    std::uniform_int_distribution<int> num_dist{0, 99};

    for (int i = 0; i < 1000; ++i) {
        std::uniform_int_distribution<std::size_t> index_dist{
            0, numbers.size() * 2
        };
        std::size_t const idx = index_dist(engine);

        if (idx < numbers.size()) {
            numbers.erase(numbers.begin() + idx);
        } else {
            numbers.insert(num_dist(engine));
        }
    }

    for (int num : numbers) {
        std::cout << num << '\n';
    }
}
