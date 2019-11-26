#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include <Eigen/Sparse>


using Matrix = Eigen::SparseMatrix<std::int32_t, Eigen::RowMajor>;


Matrix make_sparse_matrix(Matrix::Index n, Matrix::Index seed)
{
    constexpr Matrix::Index sparsity = 10;

    // Approx. the same performance as using setFromTriplets, but with less
    // memory overhead. The memory layout MUST be RowMajor for this case to
    // finish in a reasonable time.
    //
    // See also:
    // https://stackoverflow.com/questions/18154027/sparsematrix-construction-in-eigen

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


int main()
{

    constexpr Matrix::Index n = 50000;

    Matrix sum(n, n);

    for (int i = 0; i < 40; i++) {
        sum += make_sparse_matrix(n, i);
    }
}
