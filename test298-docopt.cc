#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#include <docopt/docopt.h>


constexpr char const program_usage[] =
R"(checksum

Usage:
    checksum compute [--init=<n>] <file>
    checksum verify [--init=<n>] <sum> <file>

Options:
    -h --help   Show this help and exit.
    --init=<n>  Checksum initial value [default: 0].
)";

constexpr std::size_t buffer_size = 4096;

int run(std::map<std::string, docopt::value> const& opts);
int compute(std::map<std::string, docopt::value> const& opts);
int verify(std::map<std::string, docopt::value> const& opts);
unsigned char compute_checksum(std::istream& src, unsigned char init);


int main(int argc, char** argv)
{
    try {
        return run(docopt::docopt(program_usage, {argv + 1, argv + argc}));
    } catch (std::exception const& e) {
        std::cerr << "error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}


int run(std::map<std::string, docopt::value> const& opts)
{
    if (opts.at("compute").asBool()) {
        return compute(opts);
    }

    if (opts.at("verify").asBool()) {
        return verify(opts);
    }

    assert(0);
}


int compute(std::map<std::string, docopt::value> const& opts)
{
    std::ifstream file{opts.at("<file>").asString(), std::ios::binary};
    if (not file) {
        throw std::runtime_error("cannot open file");
    }

    auto const init = static_cast<unsigned char>(opts.at("--init").asLong());
    auto const sum = compute_checksum(file, init);

    std::cout << static_cast<unsigned>(sum) << '\n';

    return 0;
}


int verify(std::map<std::string, docopt::value> const& opts)
{
    std::ifstream file{opts.at("<file>").asString(), std::ios::binary};
    if (not file) {
        throw std::runtime_error("cannot open file");
    }

    auto const init = static_cast<unsigned char>(opts.at("--init").asLong());
    auto const expected = static_cast<unsigned char>(opts.at("<sum>").asLong());
    auto const sum = compute_checksum(file, init);

    if (sum != expected) {
        std::cerr << "checksum mismatch\n";
        return 1;
    }

    return 0;
}


unsigned char compute_checksum(std::istream& src, unsigned char init)
{
    auto buf = std::vector<char>(buffer_size);
    auto sum = init;

    while (src) {
        src.read(buf.data(), buf.size());
        auto const avail_size = static_cast<std::size_t>(src.gcount());

        for (std::size_t i = 0; i < avail_size; ++i) {
            sum += static_cast<unsigned char>(buf[i]);
        }
    }

    return sum;
}
