namespace dim
{
    template<typename T, typename Dim>
    class scalar {};

    template<typename T, int N, typename Dim>
    class vector {};

    template<typename T, int N, typename Dim>
    class point {};

    template<typename T, int N, typename Dim>
    inline
    vector<T, N, Dim> operator-(point<T, N, Dim> const& lhs, point<T, N, Dim> const& rhs)
    {
        (void) lhs;
        (void) rhs;
        return {};
    }

    template<typename T, int N, typename Dim>
    inline
    scalar<T, Dim> norm(vector<T, N, Dim> const& vec)
    {
        (void) vec;
        return {};
    }

    template<int L, int T, int M, int C>
    struct mechanical_dimension;

    namespace mech
    {
        using number = mechanical_dimension<0, 0, 0, 0>;
        using length = mechanical_dimension<1, 0, 0, 0>;
        using time   = mechanical_dimension<0, 1, 0, 0>;
        using mass   = mechanical_dimension<0, 0, 1, 0>;
        using charge = mechanical_dimension<0, 0, 0, 1>;
    }
}

namespace sim
{
    using number = dim::scalar<double, dim::mech::number>;
    using length = dim::scalar<double, dim::mech::length>;
    using time   = dim::scalar<double, dim::mech::time>;
    using mass   = dim::scalar<double, dim::mech::mass>;
    using charge = dim::scalar<double, dim::mech::charge>;

    using position     = dim::point<double, 3, dim::mech::length>;
    using displacement = dim::vector<double, 3, dim::mech::length>;
}

namespace sim
{
    class particle
    {
      public:
        sim::position position() const
        {
            return {};
        }
    };
}

int main()
{
    sim::particle p1;
    sim::particle p2;

    sim::displacement const r = p1.position() - p2.position();
    sim::length const d = norm(r);
    (void) d;
}
