#if 0
exec clang++ -std=c++14 -pedantic -Wall -Wextra \
             -g -fsanitize=address,fuzzer       \
             $0
#endif

#include <stack>
#include <cstddef>
#include <cstdint>

// check_parens determines if parentheses in the given text are balanced. This
// implementation contains a bug. ;-)
bool check_parens(char const* text, std::size_t length)
{
    std::stack<char> parens;
    for (std::size_t i = 0; i < length; ++i) {
        switch (char const ch = text[i]) {
          case '(':
          case '{':
          case '[':
            parens.push(ch);
            break;
          case ')':
          case '}':
          case ']':
            if (parens.top() != ch) {
                return false;
            }
            parens.pop();
            break;
          default:
            break;
        }
    }
    return true;
}

extern "C"
int LLVMFuzzerTestOneInput(std::uint8_t const* data, std::size_t size)
{
    check_parens(reinterpret_cast<char const*>(data), size);
    return 0;
}
