#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include <Eigen/Sparse>
#include <highfive/H5Attribute.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5File.hpp>
#include <highfive/H5PropertyList.hpp>

#include "highfive_vectorarray.hpp"


using Matrix = Eigen::SparseMatrix<std::int32_t, Eigen::RowMajor>;


Matrix make_sparse_matrix(Matrix::Index n, Matrix::Index seed)
{
    constexpr Matrix::Index sparsity = 10;

    Matrix counts(n, n);
    counts.reserve(n * sparsity);

    Matrix::Index j = seed;
    for (Matrix::Index i = 0; i < n; i++) {
        for (Matrix::Index max = sparsity; max > 0; max--) {
            j = (j * j + n / 7) % n;
            counts.insert(i, j) = 1;
        }
    }

    return counts;
}


template<typename S, typename F>
void foreach_triplets(S const& matrix, F callback)
{
    for (typename S::Index i = 0; i < matrix.outerSize(); ++i) {
        for (typename S::InnerIterator it{matrix, i}; it; ++it) {
            callback(it.row(), it.col(), it.value());
        }
    }
}


int main()
{

    constexpr Matrix::Index n = 50000;

    Matrix sum(n, n);

    for (int i = 0; i < 40; i++) {
        sum += make_sparse_matrix(n, i);
    }

    HighFive::File store{
        "store.h5", HighFive::File::Create | HighFive::File::Truncate
    };

    std::size_t const sample_count = static_cast<std::size_t>(sum.nonZeros());
    std::size_t const chunk_size = 1398101;  // 16 MiB

    HighFive::DataSpace dataspace{sample_count, 3};
    HighFive::DataSetCreateProps props;
    props.add(HighFive::Chunking{chunk_size, 3});
    props.add(HighFive::Shuffle{});
    props.add(HighFive::Deflate{4}); // This is the bottleneck.
    auto dataset = store.createDataSet<std::int32_t>("matrix", dataspace, props);

    std::size_t offset = 0;
    std::vector<std::array<std::uint32_t, 3>> chunk;

    foreach_triplets(sum, [&](auto i, auto j, auto v) {
        chunk.push_back({
            std::uint32_t(i), std::uint32_t(j), std::uint32_t(v)
        });

        // This chunk size should match the dataset chunk size.
        if (chunk.size() == chunk_size) {
            dataset.select({offset, 0}, {chunk.size(), 3}).write(chunk);
            offset += chunk.size();
            std::clog << int(100 * double(offset) / double(sum.nonZeros())) << " %\n";
            chunk.clear();
        }
    });

    dataset.select({offset, 0}, {chunk.size(), 3}).write(chunk);
}
