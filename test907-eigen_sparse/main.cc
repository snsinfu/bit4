#include <iostream>
#include <vector>

#include <Eigen/Sparse>


using Matrix = Eigen::SparseMatrix<int, Eigen::RowMajor>;
using Triplet = Eigen::Triplet<int, int>;

constexpr Matrix::Index dim = 10;
constexpr Matrix::Index expected_nnz = 3;

void accumulate(Matrix& matrix);


int main()
{
    Matrix matrix;
    matrix.resize(dim, dim);
    matrix.reserve(dim * expected_nnz);

    accumulate(matrix);
    std::cout << matrix << '\n';

    // Second setFromTriplets() overwrites the previously set values.
    accumulate(matrix);
    std::cout << matrix << '\n';
}


void accumulate(Matrix& matrix)
{
    std::vector<Triplet> triplets;
    triplets.reserve(dim * expected_nnz);

    for (Matrix::Index i = 0; i < dim; i++) {
        for (Matrix::Index k = 0; k < expected_nnz; k++) {
            auto const j = (i + i * k) % dim;
            triplets.push_back({int(i), int(j), 1});
        }
    }

    matrix.setFromTriplets(triplets.begin(), triplets.end());
}
