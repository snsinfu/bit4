#ifndef INCLUDED_GTF_HPP
#define INCLUDED_GTF_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

#include "strings.hpp"


// scan_attributes parses str as a GTF attribute field. It writes key-value
// pairs to out and returns the number of pairs parsed.
template<typename OutputIterator>
inline std::size_t scan_attributes(std::string const& str, OutputIterator out)
{
    // attribute = *(key '"' value '"' ';')

    constexpr char quote_char = '"';
    constexpr char delim_char = ';';

    std::string::size_type pos = 0;
    std::size_t count = 0;

    while (pos < str.size()) {
        auto const quote1_pos = str.find(quote_char, pos);
        if (quote1_pos == std::string::npos) {
            throw std::runtime_error("no opening quote");
        }

        auto const quote2_pos = str.find(quote_char, quote1_pos + 1);
        if (quote2_pos == std::string::npos) {
            throw std::runtime_error("no closing quote");
        }

        auto const delim_pos = str.find(delim_char, quote2_pos + 1);
        if (delim_pos == std::string::npos) {
            throw std::runtime_error("no delimiter");
        }

        auto const key = strip(str.substr(pos, quote1_pos - pos));
        auto const value = str.substr(quote1_pos + 1, quote2_pos - quote1_pos - 1);

        *out++ = std::make_pair(key, value);
        pos = delim_pos + 1;
        count++;
    }

    return count;
}

#endif
