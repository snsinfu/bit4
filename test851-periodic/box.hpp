#pragma once

#include <cmath>

#include <md.hpp>


namespace detail
{
    inline md::scalar round_mod(md::scalar x, md::scalar m)
    {
        return x - std::nearbyint(x * (1 / m)) * m;
    }
}


struct open_box
{
    md::index particle_count = 1000;

    md::vector shortest_displacement(md::point p1, md::point p2) const
    {
        return p1 - p2;
    }
};


struct periodic_box
{
    md::scalar x_period = 1;
    md::scalar y_period = 1;
    md::scalar z_period = 1;

    md::vector period_vector() const
    {
        return {x_period, y_period, z_period};
    }

    md::vector shortest_displacement(md::point p1, md::point p2) const
    {
        const auto dx = detail::round_mod(p1.x - p2.x, x_period);
        const auto dy = detail::round_mod(p1.y - p2.y, y_period);
        const auto dz = detail::round_mod(p1.z - p2.z, z_period);
        return {dx, dy, dz};
    }
};


struct z_periodic_box
{
    md::scalar z_period = 1;

    md::vector period_vector() const
    {
        return {0, 0, z_period};
    }

    md::vector shortest_displacement(md::point p1, md::point p2) const
    {
        const auto dx = p1.x - p2.x;
        const auto dy = p1.y - p2.y;
        const auto dz = detail::round_mod(p1.z - p2.z, z_period);
        return {dx, dy, dz};
    }
};


struct xy_periodic_box
{
    md::scalar x_period = 1;
    md::scalar y_period = 1;

    md::vector period_vector() const
    {
        return {x_period, y_period, 0};
    }

    md::vector shortest_displacement(md::point p1, md::point p2) const
    {
        const auto dx = detail::round_mod(p1.x - p2.x, x_period);
        const auto dy = detail::round_mod(p1.y - p2.y, y_period);
        const auto dz = p1.z - p2.z;
        return {dx, dy, dz};
    }
};
