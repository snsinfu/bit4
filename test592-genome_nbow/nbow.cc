#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>


void run(std::string filename, std::size_t window_size)
{
    constexpr std::size_t base_size = 4;
    std::size_t base_index[256] = {};
    char canonical_bases[] = {'A', 'C', 'G', 'T'};

    base_index['A'] = 0;
    base_index['C'] = 1;
    base_index['G'] = 2;
    base_index['T'] = 3;

    base_index['a'] = 0;
    base_index['c'] = 1;
    base_index['g'] = 2;
    base_index['t'] = 3;

    auto const monogram = [&](char c) {
        return base_index[static_cast<unsigned char>(c)];
    };

    auto const bigram = [&](char c1, char c2) {
        auto const a1 = base_index[static_cast<unsigned char>(c1)];
        auto const a2 = base_index[static_cast<unsigned char>(c2)];
        return a1 * base_size + a2;
    };

    auto const trigram = [&](char c1, char c2, char c3) {
        auto const a1 = base_index[static_cast<unsigned char>(c1)];
        auto const a2 = base_index[static_cast<unsigned char>(c2)];
        auto const a3 = base_index[static_cast<unsigned char>(c3)];
        return (a1 * base_size + a2) * base_size + a3;
    };

    auto const quadgram = [&](char c1, char c2, char c3, char c4) {
        auto const a1 = base_index[static_cast<unsigned char>(c1)];
        auto const a2 = base_index[static_cast<unsigned char>(c2)];
        auto const a3 = base_index[static_cast<unsigned char>(c3)];
        auto const a4 = base_index[static_cast<unsigned char>(c4)];
        return ((a1 * base_size + a2) * base_size + a3) * base_size + a4;
    };


    //
    std::ifstream genome{filename};

    if (!genome) {
        throw std::runtime_error("cannot open genome file");
    }

    std::cout << "chrom\tbegin\tend";

    for (char base : canonical_bases) {
        std::cout << '\t' << base;
    }

    for (char base1 : canonical_bases) {
        for (char base2 : canonical_bases) {
            std::cout << '\t' << base1 << base2;
        }
    }

    for (char base1 : canonical_bases) {
        for (char base2 : canonical_bases) {
            for (char base3 : canonical_bases) {
                std::cout << '\t' << base1 << base2 << base3;
            }
        }
    }

    for (char base1 : canonical_bases) {
        for (char base2 : canonical_bases) {
            for (char base3 : canonical_bases) {
                for (char base4 : canonical_bases) {
                    std::cout << '\t' << base1 << base2 << base3 << base4;
                }
            }
        }
    }

    std::cout << '\n';

    std::string chrom;
    std::int64_t coord = 0;
    std::string window;

    auto const output = [&] {
        auto const base_count = std::min(window.size(), window_size);

        int histogram[4 * 4 * 4 * 4] = {};

        // Letter
        for (std::size_t i = 0; i < base_count; i++) {
            if (window[i]  == 'N' || window[i] == 'n') {
                continue;
            }
            histogram[monogram(window[i])]++;

            if (i + 1 >= window_size || window[i + 1]  == 'N' || window[i + 1] == 'n') {
                continue;
            }
            histogram[bigram(window[i], window[i + 1])]++;

            if (i + 2 >= window_size || window[i + 2]  == 'N' || window[i + 2] == 'n') {
                continue;
            }
            histogram[trigram(window[i], window[i + 1], window[i + 2])]++;

            if (i + 3 >= window_size || window[i + 3]  == 'N' || window[i + 3] == 'n') {
                continue;
            }
            histogram[quadgram(window[i], window[i + 1], window[i + 2], window[i + 3])]++;
        }

        std::cout << chrom << '\t'
                  << coord << '\t'
                  << coord + static_cast<std::int64_t>(base_count);

        coord += static_cast<std::int64_t>(base_count);

        for (char base : canonical_bases) {
            std::cout << '\t' << histogram[monogram(base)];
        }

        for (char base1 : canonical_bases) {
            for (char base2 : canonical_bases) {
                std::cout << '\t' << histogram[bigram(base1, base2)];
            }
        }

        for (char base1 : canonical_bases) {
            for (char base2 : canonical_bases) {
                for (char base3 : canonical_bases) {
                    std::cout << '\t' << histogram[trigram(base1, base2, base3)];
                }
            }
        }

        for (char base1 : canonical_bases) {
            for (char base2 : canonical_bases) {
                for (char base3 : canonical_bases) {
                    for (char base4 : canonical_bases) {
                        auto const index = quadgram(
                            base1,
                            base2,
                            base3,
                            base4
                        );
                        std::cout << '\t' << histogram[index];
                    }
                }
            }
        }

        std::cout << '\n';

        window.erase(
            window.begin(),
            window.begin() + static_cast<std::ptrdiff_t>(base_count)
        );
    };

    for (std::string line; std::getline(genome, line); ) {
        if (line.front() == '>') {
            if (!window.empty()) {
                output();
            }
            chrom = line.substr(1);
            coord = 0;
            window.clear();
            continue;
        }

        window += line;

        if (window.size() > window_size) {
            output();
        }
    }
}

int main()
{
    try {
        run("hg19.fa", 1000);
        return 0;
    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
