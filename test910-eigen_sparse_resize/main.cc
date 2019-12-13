#include <iostream>

#include <Eigen/Sparse>


int main()
{
    using Matrix = Eigen::SparseMatrix<int, Eigen::RowMajor>;

    constexpr Matrix::Index dim = 10;

    Matrix part(dim, dim);
    part.insert(1, 2) = 1;
    part.insert(2, 4) = 1;
    part.insert(3, 7) = 1;
    part.insert(4, 1) = 1;

    Matrix matrix(dim, dim);
    matrix += part;
    matrix.resize(dim, dim); // resize() zeroes matrix!
    matrix += part;

    std::cout << matrix << '\n';
}
