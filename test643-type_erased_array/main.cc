#include <cstddef>
#include <vector>


namespace
{
    class boxed_array
    {
    public:
        virtual ~boxed_array() = default;
        virtual std::size_t size() const = 0;
        virtual void resize(std::size_t n) = 0;
    };

    template<typename T>
    class boxed_array_erasure : public boxed_array
    {
    public:
        boxed_array_erasure(std::size_t n, T init)
            : values_(n), init_{init}
        {
        }

        std::size_t size() const override
        {
            return values_.size();
        }

        void resize(std::size_t n) override
        {
            values_.resize(n, init_);
        }

    private:
        std::vector<T> values_;
        T init_;
    };
}


int main()
{
}
