#include <vector>
#include <functional>


template<typename Op>
class broadcaster : Op
{
public:
    template<typename Iterator>
    void operator()(Iterator first, Iterator last) const
    {
        for (; first != last; ++first) {
            op()(*first);
        }
    }

    template<typename Range>
    void operator()(Range&& range) const
    {
        using std::begin;
        using std::end;
        operator()(begin(range), end(range));
    }

private:
    Op const& op() const
    {
        return *this;
    }
};


int main()
{
    int arr[] = {1, 2, 3, 4};
    std::vector<int> vec = {5, 6, 7};

    broadcaster<std::negate<int>> neg;
    neg(arr);
    neg(vec);
}
