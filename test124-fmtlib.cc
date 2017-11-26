#include <fmt.hpp>

namespace
{
    struct Point
    {
        double x, y, z;
    };

    void format_arg(fmt::BasicFormatter<char>& fmt, char const*& format_str, Point const& point)
    {
        // Do we really need to copy? No string_view?
        std::string format = "{";
        do {
            format += *format_str;
        } while (*format_str++ != '}');

        auto& writer = fmt.writer();
        writer.write(format, point.x);
        writer << '\t';
        writer.write(format, point.y);
        writer << '\t';
        writer.write(format, point.z);
    }
}

int main()
{
    Point point = {1.234, 5.678, 9.012};
    fmt::print("{}\n", point);
    fmt::print("{:.2f}\n", point);
    fmt::print("{:.16e}\n", point);
}
