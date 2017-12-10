#include <cerrno>
#include <cstddef>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <system_error>
#include <tuple>

#include <unistd.h>

namespace
{
    struct error
    {
        error() = default;

        explicit error(std::string msg) noexcept
            : msg_{std::move(msg)}
        {
        }

        explicit operator bool() const noexcept
        {
            return bool(msg_);
        }

        std::string_view message() const noexcept
        {
            return msg_.value();
        }

        static error from_errno() noexcept
        {
            int code = errno;
            return error{std::system_category().message(code)};
        }

      private:
        std::optional<std::string> msg_;
    };

    inline std::ostream& operator<<(std::ostream& os, error const& err)
    {
        return os << err.message();
    }
}

namespace io
{
    inline std::tuple<std::size_t, error> write(int fd, std::string_view str)
    {
        ssize_t nb_written = ::write(fd, str.data(), str.size());
        if (nb_written == -1) {
            return {0, error::from_errno()};
        }
        return {static_cast<std::size_t>(nb_written), {}};
    }
}

int main()
{
    std::string_view str = "Lorem ipsum\n";

    auto[nb_written, err] = io::write(STDOUT_FILENO, str);
    if (err) {
        std::clog << err << '\n';
        return 1;
    }
}
