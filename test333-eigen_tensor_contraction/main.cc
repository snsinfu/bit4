#include <array>
#include <iostream>

#include <Eigen/Core>
#include <Eigen/CXX11/Tensor>


int main()
{
    long const m = 20;
    long const n = 1000;
    long const r = 100;

    Eigen::Tensor<double, 2> A_ij(m, n);
    Eigen::Tensor<double, 2> B_jk(n, r);

    A_ij.setZero();
    B_jk.setZero();

    std::array<Eigen::IndexPair<long>, 1> contracted_dimensions = {{
        {1, 0},
    }};

    auto const expA_ij = A_ij.exp();
    auto const expB_jk = B_jk.exp();
    auto const C_ik = expA_ij.contract(expB_jk, contracted_dimensions);

    std::cout << C_ik << '\n';
}
