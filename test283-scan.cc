#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <system_error>


std::error_condition parse(std::string const& text, long long& val)
{
    char* endptr;
    errno = 0;
    val = std::strtoll(text.c_str(), &endptr, 10);

    if (errno) {
        return std::generic_category().default_error_condition(errno);
    }
    if (*endptr != '\0') {
        return std::errc::invalid_argument;
    }

    return {};
}

std::error_condition parse(std::string const& text, int& val)
{
    long long long_val;

    if (auto err = parse(text, long_val)) {
        return err;
    }
    if (long_val < std::numeric_limits<int>::min()) {
        return std::errc::result_out_of_range;
    }
    if (long_val > std::numeric_limits<int>::max()) {
        return std::errc::result_out_of_range;
    }
    val = static_cast<int>(long_val);

    return {};
}

int main()
{
    int num;

    if (auto err = parse("12345.67", num)) {
        std::cerr << "error: " << err.message() << '\n';
        return 1;
    }
    std::cout << num << '\n';
}
