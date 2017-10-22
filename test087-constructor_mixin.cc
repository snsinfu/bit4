// Mix in constructor from CRTP base class

template<typename Derived>
class constructor_mixin
{
  public:
    constructor_mixin() = default;

    constructor_mixin(double x, double y, double z)
    //  : Derived::values_{x, y, z} // can't
    {
        static_cast<Derived&>(*this).values_[0] = x;
        static_cast<Derived&>(*this).values_[1] = y;
        static_cast<Derived&>(*this).values_[2] = z;
    }
};

class vector : private constructor_mixin<vector>
{
    friend constructor_mixin<vector>;

  public:
    using constructor_mixin<vector>::constructor_mixin;

    double operator[](unsigned index) const
    {
        return values_[index];
    }

  private:
    double values_[3] {};
};

#include <iostream>

int main()
{
    vector v;
    vector w = {1, 2, 3};

    std::cout << v[0] << v[1] << v[2] << '\n';
    std::cout << w[0] << w[1] << w[2] << '\n'; // "000" wtf
}
