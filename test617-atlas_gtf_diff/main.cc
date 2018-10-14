#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "gtf.hpp"
#include "strings.hpp"


static void run(std::string ensembl, std::string rnaseq);
static std::vector<std::string> load_ensembl_genes(std::string filename);
static std::vector<std::string> load_rnaseq_genes(std::string filename);


int main(int argc, char** argv)
{
    enum {
        exit_ok = 0,
        exit_error = 1,
        exit_invalid_usage = 64
    };

    if (argc != 3) {
        std::cerr << "usage: main <ensembl> <rnaseq>\n";
        return exit_invalid_usage;
    }

    try {
        run(argv[1], argv[2]);
    } catch (std::exception const& ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return exit_error;
    }

    return exit_ok;
}


void run(std::string ensembl, std::string rnaseq)
{
    auto ensembl_genes = load_ensembl_genes(ensembl);
    auto rnaseq_genes = load_rnaseq_genes(rnaseq);

    std::sort(ensembl_genes.begin(), ensembl_genes.end());
    ensembl_genes.erase(
        std::unique(ensembl_genes.begin(), ensembl_genes.end()),
        ensembl_genes.end()
    );

    std::sort(rnaseq_genes.begin(), rnaseq_genes.end());
    rnaseq_genes.erase(
        std::unique(rnaseq_genes.begin(), rnaseq_genes.end()),
        rnaseq_genes.end()
    );

    // Common
    std::vector<std::string> common_genes;

    std::set_intersection(
        ensembl_genes.begin(),
        ensembl_genes.end(),
        rnaseq_genes.begin(),
        rnaseq_genes.end(),
        std::back_inserter(common_genes)
    );

    // Union
    std::vector<std::string> union_genes;

    std::set_union(
        ensembl_genes.begin(),
        ensembl_genes.end(),
        rnaseq_genes.begin(),
        rnaseq_genes.end(),
        std::back_inserter(union_genes)
    );

    std::cout << "Ensembl: " << ensembl_genes.size() << '\n';
    std::cout << "RNA-seq: " << rnaseq_genes.size() << '\n';
    std::cout << "Common: " << common_genes.size() << '\n';
    std::cout << "Union: " << union_genes.size() << '\n';
}


std::vector<std::string> load_ensembl_genes(std::string filename)
{
    enum {
        field_count = 9
    };

    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("cannot open " + filename);
    }

    std::vector<std::string> genes;
    std::array<std::string, field_count> fields;
    std::unordered_map<std::string, std::string> attrs;

    for (std::string line; std::getline(file, line); ) {
        if (!line.empty() && line.front() == '#') {
            continue;
        }

        if (split(line, '\t', field_count, fields.begin()) != field_count) {
            throw std::runtime_error("too few fields in ensembl genes file");
        }

        // NOTE: We only need gene_id attribute so scanning all attributes (with
        // unordered_map allocations!) is horribly inefficient.
        attrs.clear();
        scan_attributes(fields.at(8), std::inserter(attrs, attrs.end()));

        auto const gene_id_entry = attrs.find("gene_id");
        if (gene_id_entry != attrs.end()) {
            genes.push_back(gene_id_entry->second);
        }
    }

    return genes;
}


std::vector<std::string> load_rnaseq_genes(std::string filename)
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("cannot open " + filename);
    }

    std::string header;
    std::getline(file, header);

    std::vector<std::string> genes;

    for (std::string line; std::getline(file, line); ) {
        auto const delim_pos = line.find('\t');
        if (delim_pos == std::string::npos) {
            throw std::runtime_error("invalid row");
        }

        genes.push_back(line.substr(0, delim_pos));
    }

    return genes;
}
