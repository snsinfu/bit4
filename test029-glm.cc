#include <iostream>
#include <ratio>

#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/vector_relational.hpp>

#include <glm/gtc/integer.hpp>
#include <glm/gtc/ulp.hpp>

#include <glm/gtx/hash.hpp>
#include <glm/gtx/range.hpp>

#ifdef WITH_UNITS
# include <units.h>
#endif

namespace sim {

#ifdef WITH_UNITS
    namespace abstract {
        using scalar = units::unit<std::ratio<1>, units::category::scalar_unit>;
        using length = units::unit<std::ratio<1>, units::category::length_unit>;
    }
    using scalar_t = units::unit_t<abstract::scalar>;
    using length_t = units::unit_t<abstract::length>;
#else
    using scalar_t = double;
    using length_t = double;
#endif

    using point = glm::tvec3<sim::length_t>;

    using vector = glm::tvec3<sim::scalar_t>;
    using matrix = glm::tmat3x3<sim::scalar_t>;

    using length_vec = glm::tvec3<sim::length_t>;

    // GLM_Core
    using glm::cross;
    using glm::distance;
    using glm::dot;
    using glm::faceforward;
    using glm::length;
    using glm::length;
    using glm::normalize;
    using glm::reflect;
    using glm::refract;

    using glm::all;
    using glm::any;
    using glm::equal;
    using glm::not_;

    template<typename T, glm::precision P, template<typename, glm::precision> class Vec>
    inline Vec<bool, P> greater_than(Vec<T, P> const& x, Vec<T, P> const& y) {
        return glm::greaterThan(x, y);
    }

    template<typename T, glm::precision P, template<typename, glm::precision> class Vec>
    inline Vec<bool, P> greater_than_equal(Vec<T, P> const& x, Vec<T, P> const& y) {
        return glm::greaterThanEqual(x, y);
    }

    template<typename T, glm::precision P, template<typename, glm::precision> class Vec>
    inline Vec<bool, P> less_than(Vec<T, P> const& x, Vec<T, P> const& y) {
        return glm::lessThan(x, y);
    }

    template<typename T, glm::precision P, template<typename, glm::precision> class Vec>
    inline Vec<bool, P> less_than_equal(Vec<T, P> const& x, Vec<T, P> const& y) {
        return glm::lessThanEqual(x, y);
    }

    template<typename T, glm::precision P, template<typename, glm::precision> class Vec>
    inline Vec<bool, P> not_equal(Vec<T, P> const& x, Vec<T, P> const& y) {
        return glm::notEqual(x, y);
    }

    // GLM_GTC_integer
    using glm::iround;
    using glm::log2;
    using glm::mod;
    using glm::uround;

    // GLM_GTC_ulp
    using glm::float_distance;
    using glm::next_float;
    using glm::prev_float;
}

int main() {
    sim::point p, q, r;
    sim::length_vec v = sim::cross(p - q, q - r);

    std::cout << sim::length(v) << '\n';

    for (auto e : v) {
        std::cout << e << '\n';
    }
}
