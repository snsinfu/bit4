#include <array>
#include <iostream>

#include <Eigen/Core>
#include <Eigen/CXX11/Tensor>
#include <Eigen/Dense>


int main()
{
    Eigen::Tensor<double, 2> distr(3, 7);

    distr.setValues({
        //   01  11  21  31  41  51  61
            { 1,  2,  3,  4,  5,  6,  7}, // 00
            { 8,  9, 10, 11, 12, 13, 14}, // 10
            {15, 16, 17, 18, 19, 20, 21}, // 20
    });

    Eigen::Tensor<double, 2> distr_digamma = distr.digamma();
    std::array<int, 1> const along_second_axis{{1}};
    Eigen::Tensor<double, 1> sum_digamma = distr.sum(along_second_axis).digamma();
    Eigen::Tensor<double, 2> distr_geometric_expect = distr.digamma();

    for (long i = 0; i < distr.dimension(0); ++i) {
        for (long j = 0; j < distr.dimension(1); ++j) {
            distr_geometric_expect(i, j) -= sum_digamma(i);
        }
    }
}
