#include <iostream>

#include <Eigen/Core>
#include <Eigen/CXX11/Tensor>


int main()
{
    using DataTensor = Eigen::Tensor<double, 3>;

    DataTensor data(2, 3, 4);
    data.setZero();

    std::cout << data << '\n';
}
