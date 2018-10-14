#ifndef INCLUDED_STRINGS_HPP
#define INCLUDED_STRINGS_HPP

#include <cstddef>
#include <cctype>
#include <string>


// split splits str into tokens delimited by delim upto limit tokens. It writes
// tokens to out and returns the number of tokens parsed.
template<typename OutputIterator>
inline std::size_t split(std::string const& str, char delim, std::size_t limit, OutputIterator out)
{
    std::size_t count = 0;
    std::size_t pos = 0;

    while (pos < str.size() && count < limit) {
        auto next_pos = str.find(delim, pos);
        if (next_pos == std::string::npos) {
            next_pos = str.size();
        }

        *out++ = str.substr(pos, next_pos - pos);
        pos = next_pos + 1;
        count++;
    }

    return count;
}


// strip strips leading and trailing whitespaces of str. It used std::isblank to
// determine if a character is considered a whitespace.
inline std::string strip(std::string const& str)
{
    std::string::size_type beg = 0;

    while (beg < str.size()) {
        if (!std::isblank(str[beg])) {
            break;
        }
        beg++;
    }

    std::string::size_type end = str.size();

    while (end > 0) {
        if (!std::isblank(str[end - 1])) {
            break;
        }
        end--;
    }

    return str.substr(beg, end - beg);
}


#endif
