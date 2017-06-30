#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include <gsl/span>


struct Interval {

    Interval() = default;

    Interval(double begin, double end)
        : begin_{begin}, end_{end} {
    }

    double begin() const {
        return begin_;
    }

    double end() const {
        return end_;
    }

  private:
    double begin_ = 0;
    double end_   = 0;
};

void report(int i, int j) {
    static int dummy;
    dummy += i;
    dummy -= j;
}

void naive(gsl::span<Interval const> intervals) {

    auto contains = [](Interval const& iv, double p) {
        return iv.begin() <= p && p < iv.end();
    };

    auto intersects = [&](Interval const& a, Interval const& b) {
        return contains(a, b.begin()) || contains(b, a.begin());
    };

    for (int i = 0; i < intervals.size(); ++i) {
        for (int j = i + 1; j < intervals.size(); ++j) {
            if (intersects(intervals[i], intervals[j])) {
                report(i, j);
            }
        }
    }
}

void sweep(gsl::span<Interval const> intervals) {

    struct Point {
        enum class Kind {
            begin, end
        };
        double pos;
        int    index;
        Kind   kind;
    };

    std::vector<Point> points;
    points.reserve(intervals.size() * 2);

    for (int i = 0; i < intervals.size(); ++i) {
        points.push_back({ intervals[i].begin(), i, Point::Kind::begin });
        points.push_back({ intervals[i].end(),   i, Point::Kind::end   });
    }

    std::sort(points.begin(), points.end(), [](Point const& x, Point const& y) {
        return x.pos < y.pos;
    });

    std::vector<int> slot_map(intervals.size());
    std::vector<int> active;

    for (auto e : points) {
        if (e.kind == Point::Kind::begin) {
            for (auto i : active) {
                report(e.index, i);
            }
        }

        switch (e.kind) {
          case Point::Kind::begin:
            slot_map[e.index] = static_cast<int>(active.size());
            active.push_back(e.index);
            break;

          case Point::Kind::end:
            active[slot_map[e.index]] = active.back();
            active.pop_back();
            break;
        }
    }
}

template<typename Op>
void timeit(Op op) {
    using clock = std::chrono::high_resolution_clock;

    auto const start = clock::now();
    op();
    auto const elapsed = clock::now() - start;

    std::clog << std::chrono::duration<double>(elapsed).count() << '\n';
}

int main() {
    std::vector<Interval> intervals;

    std::mt19937_64 engine;
    std::normal_distribution<double> beg_dist{0.0, 1.0};
    std::gamma_distribution<double> len_dist{10.0, 0.1};

    for (int i = 0; i < 30000; ++i) {
        auto const beg = beg_dist(engine);
        auto const len = len_dist(engine);
        intervals.push_back({beg, beg + len});
    }

    // Warm up cache memory
    for (int i = 0; i < 5; ++i) {
        sweep(intervals);
    }

    timeit([&]() { sweep(intervals); });
    timeit([&]() { naive(intervals); });
}
