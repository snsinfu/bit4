#include <cassert>
#include <vector>

struct Summarizer
{
    virtual
    ~Summarizer() = default;

    virtual
    int summarize(std::vector<int> const& values) const = 0;
};

template<typename Derived>
struct PairSummarizer : Summarizer
{
    using Index = std::vector<int>::size_type;

    virtual
    int summarize(std::vector<int> const& values) const override
    {
        int sum = 0;

        for (Index j = 0; j < values.size(); ++j) {
            for (Index i = 0; i < j; ++i) {
                sum += derived().evaluate_pair(values[i], values[j]);
            }
        }
        return sum;
    }

  private:
    int evaluate_pair(int x, int y) const = delete; // implement this in the derived class!

  private:
    Derived const& derived() const
    {
        return static_cast<Derived const&>(*this);
    }
};

struct PairProductSummarizer : PairSummarizer<PairProductSummarizer>
{
    //int evaluate_pair(int x, int y) const
    //{
    //    return x * y;
    //}
};

void test_summarizer(
    Summarizer const&       summarizer,
    std::vector<int> const& values,
    int                     expected)
{
    assert(summarizer.summarize(values) == expected);
}

int main()
{
    PairProductSummarizer pps;
    test_summarizer(pps, {1, 2, 3, 4}, 35);
}
