#include <iostream>
#include <stdexcept>
#include <string>


static void parse_range(std::string const& str);


int main()
{
    try {
        parse_range("123");
        parse_range("123-456");
        parse_range("123x");
    } catch (std::logic_error const& err) {
        std::cerr << "error: " << err.what() << '\n';
    }
}


static
void parse_range(std::string const& str)
{
    std::size_t pos;
    long start = std::stol(str, &pos);
    long end = start;

    if (pos != str.size()) {
        if (str[pos] == '-') {
            end = std::stol(str.substr(pos + 1));
        } else {
            throw std::invalid_argument("invalid range specification");
        }
    }

    std::cout << start << " - " << end << '\n';
}
