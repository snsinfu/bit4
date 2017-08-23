#include <cstddef>

namespace mod
{
    enum class position_tag : unsigned long long;
    enum class velocity_tag : unsigned long long;

    namespace literals
    {
        inline constexpr
        position_tag operator""_position(char const*, std::size_t)
        {
            return static_cast<position_tag>(0);
        }

        inline constexpr
        velocity_tag operator""_velocity(char const*, std::size_t)
        {
            return static_cast<velocity_tag>(0);
        }
    }

    template<position_tag>
    void property()
    {
    }

    template<velocity_tag>
    void property()
    {
    }
}

int main()
{
    using namespace mod::literals;

    mod::property<"A"_position>();
    mod::property<"A"_velocity>();

    mod::property<"B"_position>();
    mod::property<"B"_velocity>();
}
