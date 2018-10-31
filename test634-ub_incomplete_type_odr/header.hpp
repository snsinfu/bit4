#include <cstddef>

inline void trigger(struct incomplete*)
{
}

template<typename T>
inline std::size_t hash(void(*)(T*))
{
    return typeid(T*).hash_code();
}
