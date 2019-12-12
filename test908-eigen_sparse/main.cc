#include <iostream>
#include <vector>

#include <Eigen/Sparse>


int main()
{
    using Matrix = Eigen::SparseMatrix<int, Eigen::RowMajor>;
    using Triplet = Eigen::Triplet<int, int>;

    constexpr Matrix::Index dim = 10000;
    constexpr Matrix::Index density = 10;

    Matrix matrix;
    matrix.resize(dim, dim);

    Matrix part(dim, dim);
    std::vector<Triplet> part_triplets;

    auto accumulate = [&] {
        part.setZero();
        part_triplets.clear();

        for (Matrix::Index i = 0; i < dim; i++) {
            for (Matrix::Index k = 0; k < density; k++) {
                auto const j = (i + i * k) % dim;
                part_triplets.push_back({int(i), int(j), 1});
            }
        }

        part.setFromTriplets(part_triplets.begin(), part_triplets.end());

        matrix += part;
    };

    for (int i = 0; i < 1000; i++) {
        accumulate();
    }
    std::cout << matrix.sum() << '\n';
}
