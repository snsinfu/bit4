// Is it legal to enclose C functions in a namespace?

#include <string_view>

namespace posix
{
    extern "C"
    {
        using size_t = unsigned long;
        using ssize_t = long;

        inline constexpr int STDOUT_FILENO = 0;

        ssize_t write(int fd, char const* buf, size_t nbyte) noexcept;
    }
}

int main()
{
    std::string_view str = "Lorem ipsum\n";
    posix::write(posix::STDOUT_FILENO, str.data(), str.size());
}
