// ...or even, is it legal to enclose C includes in a namespace?
//
// https://stackoverflow.com/questions/21521333/including-c-headers-in-a-c-namespace-is-it-a-standard-behavior
//
// Maybe legal? But it does not seem to be considered a good practice.

#include <string_view>

namespace posix { extern "C" {
#include <unistd.h>
}}

int main()
{
    std::string_view msg = "Hello, world\n";
    posix::write(STDOUT_FILENO, msg.data(), msg.size());
}
