#include <unordered_map>


// typeid is tricky for (pointer to) incomplete type in clang++. Use this trick
// instead.

using typehash_t = void const*;

template<typename T>
typehash_t hash_type()
{
    static char dummy;
    return &dummy;
}

// typenam maps static type to run-time value of type T.

template<typename T>
class typemap
{
public:
    template<typename Key>
    void set(T const& value)
    {
        map_.emplace(hash_type<Key>(), value);
    }

    template<typename Key>
    T const& get() const
    {
        return map_.at(hash_type<Key>());
    }

private:
    std::unordered_map<typehash_t, T> map_;
};
