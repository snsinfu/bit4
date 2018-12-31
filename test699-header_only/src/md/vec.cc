#include <cmath>

#include <md/config.hpp>
#include <md/vec.hpp>


MD_IMPL
double md::vec::dot(md::vec const& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

MD_IMPL
double md::vec::norm() const
{
    return std::sqrt(dot(*this));
}

MD_IMPL
md::vec md::operator+(md::vec const& v1, md::vec const& v2)
{
    return md::vec{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

MD_IMPL
md::vec md::operator-(md::vec const& v1, md::vec const& v2)
{
    return md::vec{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

MD_IMPL
md::vec md::operator*(double a, md::vec const& v)
{
    return md::vec{a * v.x, a * v.y, a * v.z};
}
