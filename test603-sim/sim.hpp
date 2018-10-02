#ifndef INCLUDED_SIM_HPP
#define INCLUDED_SIM_HPP

// sim {
//   class array_view
//   class vector
//   class point
//
//   class forcefield
//   class system
//
//   class composite_forcefield
//   class pairwise_forcefield
//   class sequential_pair_forcefield
//
//   struct harmonic_potential
//   struct spring_potential
//   struct lennard_jones_potential
//   struct softcore_potential
//   struct softcore_lennard_jones_potential
//
//   simulate_newtonian_dynamics()
//   simulate_langevin_dynamics()
//   simulate_brownian_dynamics()
// }

//------------------------------------------------------------------------------
// vendor - array_view
//------------------------------------------------------------------------------

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace sim
{
    namespace array_view_detail
    {
        // data returns a pointer to the first element of a contiguous container
        // or an array.
        template<typename Cont>
        constexpr auto data(Cont& cont) noexcept -> decltype(cont.data())
        {
            return cont.data();
        }

        template<typename T, std::size_t N>
        constexpr auto data(T(& arr)[N]) noexcept -> T*
        {
            return arr;
        }

        // size returns the number of elements in a container or an array.
        template<typename Cont>
        constexpr auto size(Cont const& cont) noexcept -> decltype(cont.size())
        {
            return cont.size();
        }

        template<typename T, std::size_t N>
        constexpr auto size(T(&)[N]) noexcept -> std::size_t
        {
            return N;
        }
    }

    // array_view is an non-owning range view of an array. It is similar to
    // GSL's span but uses size_t for indexing, for better compatibility with
    // the standard library.
    template<typename T>
    class array_view
    {
    public:
        // value_type is the non-qualified type of the elements.
        using value_type = typename std::remove_cv<T>::type;

        // pointer is the type of a pointer to an element.
        using pointer = T*;

        // reference is the type of a reference to an element.
        using reference = T&;

        // size_type is the type of size and index values.
        using size_type = std::size_t;

        // iterator is the type of iterators. Guaranteed to be a random access
        // iterator. Currently this is implemented as an alias of T* but this
        // may change in the future.
        using iterator = T*;

        // reverse_iterator is the type of reverse iterators. Guaranteed to be
        // a random access iterator.
        using reverse_iterator = std::reverse_iterator<iterator>;

        // const_array_view is the type of read-only array_view with the same
        // value_type. This is the same as array_view<T> if T is already const
        // qualified.
        using const_array_view = array_view<T const>;

        // array_view is default constructible.
        array_view() = default;

        // array_view is copy constructible.
        array_view(array_view&) = default;

        // array_view is copy constructible.
        array_view(array_view const&) = default;

        // array_view is constructible from a raw memory span [p, p + n). The
        // behavior is undefined if the memory region is invalid.
        constexpr array_view(pointer p, size_type n)
            : data_{p}, size_{n}
        {
        }

        // array_view is implicitly convertible from a contiguous container like
        // std::vector.
        template<
            typename Container,
            typename P = decltype(array_view_detail::data(std::declval<Container&>())),
            typename S = decltype(array_view_detail::size(std::declval<Container&>())),
            typename = typename std::enable_if<
                std::is_convertible<typename std::remove_pointer<P>::type(*)[], T(*)[]>::value,
                int
            >::type,
            typename = typename std::enable_if<
                !std::is_same<Container, array_view<value_type>>::value,
                int
            >::type
        >
        constexpr array_view(Container& container) noexcept
            : array_view{array_view_detail::data(container), array_view_detail::size(container)}
        {
        }

        // array_view is implicitly convertible to a read-only view.
        constexpr operator const_array_view() const noexcept
        {
            return {data(), size()};
        }

        // empty returns true if the view is empty.
        constexpr bool empty() const noexcept
        {
            return size() == 0;
        }

        // size returns the number of elements.
        constexpr size_type size() const noexcept
        {
            return size_;
        }

        // data r eturns a pointer to the first element. The pointer may or may
        // not be null if the view is empty.
        constexpr pointer data() const noexcept
        {
            return data_;
        }

        // front returns a reference to the first element. The behavior is
        // undefined if the view is empty.
        constexpr reference front() const
        {
            return operator[](0);
        }

        // back returns a reference to the last element. The behavior is
        // undefined if the view is empty.
        constexpr reference back() const
        {
            return operator[](size() - 1);
        }

        // Indexing operator returns a reference to the idx-th element. The
        // behavior is undefined if the index is out of bounds.
        constexpr reference operator[](size_type idx) const
        {
            return data()[idx];
        }

        // at returns a reference to the idx-th element. It throws
        // std::out_of_range if the index is out of bounds.
        reference at(size_type idx) const
        {
            if (idx >= size()) {
                throw std::out_of_range("array_view access out-of-bounds");
            }
            return operator[](idx);
        }

        // begin returns an iterator to the beginning of the viewed array.
        constexpr iterator begin() const noexcept
        {
            return data();
        }

        // end returns an iterator to the end of the viewed array.
        constexpr iterator end() const noexcept
        {
            return data() + size();
        }

        // rbegin returns a reverse iterator to the reverse beginning.
        reverse_iterator rbegin() const noexcept
        {
            return reverse_iterator{end()};
        }

        // rend returns a reverse iterator to the reverse end.
        reverse_iterator rend() const noexcept
        {
            return reverse_iterator{begin()};
        }

        // as_const returns a read-only view of the same array.
        constexpr const_array_view as_const() const noexcept
        {
            return {data(), size()};
        }

        // swap swaps the viewed array with other.
        void swap(array_view& other) noexcept
        {
            auto const copy = *this;
            *this = other;
            other = copy;
        }

        // subview returns a view of the subarray with given region.
        constexpr array_view subview(size_type offset, size_type count) const
        {
            return {data() + offset, count};
        }

        // first returns a view of the first count elements.
        constexpr array_view first(size_type count) const
        {
            return subview(0, count);
        }

        /// last returns a view of the last count elements.
        constexpr array_view last(size_type count) const
        {
            return subview(size() - count, count);
        }

    private:
        pointer data_ = nullptr;
        size_type size_ = 0;
    };
}


//------------------------------------------------------------------------------
// geometry
//------------------------------------------------------------------------------

#include <cmath>
#include <cstddef>
#include <istream>
#include <iterator>
#include <ostream>
#include <utility>


namespace sim { namespace geo
{
    // index is the integral type used for indexing vector/point coordinate.
    using index = std::size_t;

    namespace detail
    {
        // identity is the identity metafunction. This is used to get around
        // ambiguous template type deduction errors.
        template<typename T>
        struct identity
        {
            using type = T;
        };

        template<typename T>
        using identity_t = typename identity<T>::type;

        // type_sequence holds a compile-time sequence of types.
        template<typename...>
        struct type_sequence;

        // repeat creates a type_sequence of N copies of T.
        template<typename T, index N, typename... Ts>
        struct repeat
        {
            using type = typename repeat<T, N - 1, T, Ts...>::type;
        };

        template<typename T, typename... Ts>
        struct repeat<T, 0, Ts...>
        {
            using type = type_sequence<Ts...>;
        };

        template<typename T, index N>
        using repeat_t = typename repeat<T, N>::type;
    }

    // basic_coords is a static container of N coordinate values. This is the
    // base class of vector and point.
    template<typename T, index N, typename = detail::repeat_t<T, N>>
    class basic_coords;

    template<typename T, index N, typename... Ts>
    class basic_coords<T, N, detail::type_sequence<Ts...>>
    {
    public:
        // scalar_type is the type T of coordinate values.
        using scalar_type = T;

        // iterator is the type of an iterator used to iterate over coordinate
        // values.
        using iterator = scalar_type*;

        // const_iterator is the type of an iterator used to iterate over
        // coordinate values with only read access.
        using const_iterator = scalar_type const*;

        // dimension is the number N of coordinate values.
        static constexpr index dimension = N;

        // Default constructor initializes all coordinate values to zero.
        basic_coords() = default;

        // This constructor takes exactly N coordinate values.
        basic_coords(Ts... coords) noexcept
            : coords_{coords...}
        {
        }

        // begin returns a random access iterator pointing to the first
        // coordinate value.
        iterator begin() noexcept
        {
            return &coords_[0];
        }

        const_iterator begin() const noexcept
        {
            return &coords_[0];
        }

        // end returns a random access iterator pointing to past the last
        // coordinate value.
        iterator end() noexcept
        {
            return &coords_[0] + dimension;
        }

        const_iterator end() const noexcept
        {
            return &coords_[0] + dimension;
        }

        // Indexing operator returns a reference to the i-th coordinate value.
        // Behavior is undefined if the index is out of bounds.
        scalar_type& operator[](index i)
        {
            return coords_[i];
        }

        scalar_type const& operator[](index i) const
        {
            return coords_[i];
        }

    private:
        scalar_type coords_[dimension] {};
    };

    // Equality comparison `u == v` compares the coordinate values for *exact*
    // floating-point equality.
    template<typename T, index N>
    bool operator==(basic_coords<T, N> const& u, basic_coords<T, N> const& v) noexcept
    {
        for (index i = 0; i < N; ++i) {
            if (u[i] != v[i]) {
                return false;
            }
        }
        return true;
    }

    // Inequality comparison `u != v` compares the coordinate values for *exact*
    // floating-point inequality.
    template<typename T, index N>
    bool operator!=(basic_coords<T, N> const& u, basic_coords<T, N> const& v) noexcept
    {
        return !(u == v);
    }

    // Stream-out operator for basic_coords prints the coordinate values
    // delimited by spaces.
    template<typename Char, typename Tr, typename T, index N>
    std::basic_ostream<Char, Tr>& operator<<(
        std::basic_ostream<Char, Tr>& os,
        basic_coords<T, N> const& coords
    )
    {
        using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

        if (sentry_type sentry{os}) {
            auto const delim = os.widen(' ');

            for (index i = 0; i < N; ++i) {
                if (i != 0) {
                    os << delim;
                }
                os << coords[i];
            }
        }

        return os;
    }

    // Stream-in operator for basic_coords scans coordinate values delimited by
    // spaces.
    template<typename Char, typename Tr, typename T, index N>
    std::basic_istream<Char, Tr>& operator>>(
        std::basic_istream<Char, Tr>& is,
        basic_coords<T, N>& coords
    )
    {
        using sentry_type = typename std::basic_istream<Char, Tr>::sentry;

        if (sentry_type sentry{is}) {
            for (index i = 0; i < N; ++i) {
                is >> coords[i];
            }
        }

        return is;
    }

    // vector is a linear vector in the N-dimensional Euclidean space.
    template<typename T, index N>
    class vector : public basic_coords<T, N>
    {
        using basic_coords_ = basic_coords<T, N>;

    public:
        using typename basic_coords_::scalar_type;
        using typename basic_coords_::iterator;
        using typename basic_coords_::const_iterator;
        using basic_coords_::dimension;
        using basic_coords_::basic_coords_;
        using basic_coords_::begin;
        using basic_coords_::end;
        using basic_coords_::operator[];

        // Addition works component-wise.
        vector& operator+=(vector const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] += v[i];
            }
            return *this;
        }

        // Subtraction works component-wise.
        vector& operator-=(vector const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] -= v[i];
            }
            return *this;
        }

        // Multiplication broadcasts to all components.
        vector& operator*=(scalar_type a) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] *= a;
            }
            return *this;
        }

        // Division broadcasts to all components. Behavior is undefined if a is
        // zero.
        vector& operator/=(scalar_type a)
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] /= a;
            }
            return *this;
        }

        // dot returns the dot product of this vector and v.
        scalar_type dot(vector const& v) const noexcept
        {
            scalar_type sum = 0;
            for (index i = 0; i < dimension; ++i) {
                sum += (*this)[i] * v[i];
            }
            return sum;
        }

        // squared_norm returns the squared Euclidean norm of this vector. This
        // function is faster to compute than norm.
        scalar_type squared_norm() const noexcept
        {
            return dot(*this);
        }

        // norm returns the Euclidean norm of this vector.
        scalar_type norm() const noexcept
        {
            return std::sqrt(squared_norm());
        }

        // cross returns the cross product of this vector and v. Compilation
        // fails if this function is called on a non-three-dimensional vector.
        vector cross(vector const& v) const noexcept
        {
            static_assert(
                dimension == 3,
                "cross product is defined only for three-dimensional vectors"
            );
            return vector{
                (*this)[1] * v[2] - (*this)[2] * v[1],
                (*this)[2] * v[0] - (*this)[0] * v[2],
                (*this)[0] * v[1] - (*this)[1] * v[0]
            };
        }

        // normalize returns the unit vector that is parallel to this vector.
        // Behavior is undefined if this vector is zero.
        vector normalize() const
        {
            return *this * (1 / norm());
        }
    };

    // Identity `+v` returns a copy of vec.
    template<typename T, index N>
    vector<T, N> operator+(vector<T, N> const& v) noexcept
    {
        return v;
    }

    // Negation `-v` returns a negated vector.
    template<typename T, index N>
    vector<T, N> operator-(vector<T, N> const& v) noexcept
    {
        vector<T, N> neg{v};
        for (T& coord : neg) {
            coord = -coord;
        }
        return neg;
    }

    // Addition `u + v` returns a vector whose coordinates are the element-wise
    // sum of u and v.
    template<typename T, index N>
    vector<T, N> operator+(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{u} += v;
    }

    // Subtraction `u - v` returns a vector whose coordinates are the
    // element-wise difference of u and v.
    template<typename T, index N>
    vector<T, N> operator-(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{u} -= v;
    }

    // Multiplication by scalar `v * a` returns a vector v scaled by a.
    template<typename T, index N>
    vector<T, N> operator*(vector<T, N> const& v, detail::identity_t<T> a) noexcept
    {
        return vector<T, N>{v} *= a;
    }

    // Multiplication by scalar `a * v` returns a vector v scaled by a.
    template<typename T, index N>
    vector<T, N> operator*(detail::identity_t<T> a, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{v} *= a;
    }

    // Division by scalar `v / a` returns a vector v scaled by 1/a. Behavior is
    // undefined if a is zero.
    template<typename T, index N>
    vector<T, N> operator/(vector<T, N> const& v, detail::identity_t<T> a)
    {
        return vector<T, N>{v} /= a;
    }

    // dot returns the dot product of a pair of vectors. This is the same as the
    // member function `u.dot(v)`.
    template<typename T, index N>
    T dot(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return u.dot(v);
    }

    // squared_norm returns the squared Euclidean norm of a vector. This is the
    // same as the member function `v.squared_norm()`.
    template<typename T, index N>
    T squared_norm(vector<T, N> const& v) noexcept
    {
        return v.squared_norm();
    }

    // norm returns the Euclidean norm of a vector. This is the same as the
    // member function `v.norm()`.
    template<typename T, index N>
    T norm(vector<T, N> const& v) noexcept
    {
        return v.norm();
    }

    // cross returns the cross product of a pair of three-dimensional vectors.
    // This is the same as the member function `u.cross(v)`.
    template<typename T>
    vector<T, 3> cross(vector<T, 3> const& u, vector<T, 3> const& v) noexcept
    {
        return u.cross(v);
    }

    // normalize returns the unit vector parallel to v. Behavior is undefined if
    // v is a zero vector.  This is the same as the member function
    // `v.normalize()`.
    template<typename T, index N>
    vector<T, N> normalize(vector<T, N> const& v)
    {
        return v.normalize();
    }

    // point is a geometric point in the N-dimensional Euclidean space.
    template<typename T, index N>
    class point : public basic_coords<T, N>
    {
        using basic_coords_ = basic_coords<T, N>;

    public:
        using typename basic_coords_::scalar_type;
        using typename basic_coords_::iterator;
        using typename basic_coords_::const_iterator;
        using basic_coords_::dimension;
        using basic_coords_::basic_coords_;
        using basic_coords_::begin;
        using basic_coords_::end;
        using basic_coords_::operator[];
        using vector_type = geo::vector<T, N>;

        // vector returns the coordinate vector of this point.
        vector_type vector() const noexcept
        {
            return *this - point{};
        }

        // Addition of a vector displaces this point by v.
        point& operator+=(vector_type const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] += v[i];
            }
            return *this;
        }

        // Subtraction of a vector displaces this point by -v.
        point& operator-=(vector_type const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] -= v[i];
            }
            return *this;
        }

        // squared_distance returns the squared Euclidean distance to p. This
        // function is faster to
        // compute than distance.
        scalar_type squared_distance(point const& p) const noexcept
        {
            return (*this - p).squared_norm();
        }

        // distance returns the Euclidean distance to p.
        scalar_type distance(point const& p) const noexcept
        {
            return (*this - p).norm();
        }

    };

    // Addition `p + v` returns the point pointed from p by v.
    template<typename T, index N>
    point<T, N> operator+(point<T, N> const& p, vector<T, N> const& v) noexcept
    {
        return point<T, N>{p} += v;
    }

    // Subtraction `p - v` returns the point that points to p by v.
    template<typename T, index N>
    point<T, N> operator-(point<T, N> const& p, vector<T, N> const& v) noexcept
    {
        return point<T, N>{p} -= v;
    }

    // Subtraction `p - q ` returns the displacement vector originates at q and
    // pointing to p.
    template<typename T, index N>
    vector<T, N> operator-(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        vector<T, N> vec;
        for (index i = 0; i < N; ++i) {
            vec[i] = p[i] - q[i];
        }
        return vec;
    }

    // squared_distance returns the squared Euclidean distance between two
    // points. This function is faster to compute than distance.
    template<typename T, index N>
    T squared_distance(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        return p.squared_distance(q);
    }

    // distance returns the Euclidean distance between two points.
    template<typename T, index N>
    T distance(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        return p.distance(q);
    }
}}


//------------------------------------------------------------------------------
// neighbor_searcher
//------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>


namespace sim { namespace geo
{
    // neighbor_searcher is a data structure for efficiently searching pairs of
    // points that are within a fixed "cutoff" distance in the three-dimensional
    // space.
    template<typename T>
    class neighbor_searcher
    {
    public:
        using scalar_type = T;
        using point_type = geo::point<scalar_type, 3>;
        using index_type = std::size_t;

        // Constructor takes the cutoff distance and the number of bins used for
        // hashing points.
        //
        // The optimal number of bins depends on the actual distribution of
        // points. A rule-of-thumb number for a dense system is the number of
        // points divided by 20.
        neighbor_searcher(scalar_type dcut, index_type bins)
            : dcut_(dcut), bins_(bins)
        {
            // Pre-compute the hash index differences between adjacent bins. We
            // use a linear hash function so the differences are the same (i.e.,
            // reusable) for all bins.

            index_type const index_deltas[] = {bins - 1, bins, bins + 1};

            for (auto dx : index_deltas) {
                for (auto dy : index_deltas) {
                    for (auto dz : index_deltas) {
                        hash_deltas_.push_back(linear_hash(dx, dy, dz));
                    }
                }
            }

            std::sort(hash_deltas_.begin(), hash_deltas_.end());
            hash_deltas_.erase(
                std::unique(hash_deltas_.begin(), hash_deltas_.end()),
                hash_deltas_.end()
            );
        }

        // set_points clears internal hash table and assigns given points to the
        // hash table.
        void set_points(std::vector<point_type> const& points)
        {
            for (hash_bin& bin : bins_) {
                bin.members.clear();
            }

            for (index_type idx = 0; idx < points.size(); ++idx) {
                point_type const point = points[idx];
                hash_bin& bin = bins_[locate_bin(point)];
                bin.members.push_back({idx, point});
            }
        }

        // search searches for neighbor pairs in the hash table and outputs
        // index pairs to given output iterator. Each index pair (i,j) satisfies
        // i < j and no duplicate pairs are output.
        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            index_type const bin_count = bins_.size();

            for (index_type center = 0; center < bin_count; ++center) {
                hash_bin const& center_bin = bins_[center];

                for (index_type const delta : hash_deltas_) {
                    hash_bin const& other_bin = bins_[(center + delta) % bin_count];

                    search_among(center_bin, other_bin, out);
                }
            }
        }

    private:
        struct hash_bin
        {
            struct member
            {
                index_type index;
                point_type point;
            };
            std::vector<member> members;
        };

        // Prime hash coefficients.
        static constexpr index_type x_stride = 73856093;
        static constexpr index_type y_stride = 19349669;
        static constexpr index_type z_stride = 83492791;

        // search_among searches for pairs of neighbor points in given bins. It
        // outputs index pairs to out. Each index pair (i,j) satisfies i < j
        // and no duplicate pairs are output.
        template<typename OutputIterator>
        inline void search_among(
            hash_bin const& center_bin,
            hash_bin const& other_bin,
            OutputIterator& out
        ) const
        {
            scalar_type const dcut2 = dcut_ * dcut_;

            for (auto member_i : other_bin.members) {
                for (auto member_j : center_bin.members) {
                    if (member_i.index >= member_j.index) {
                        continue;
                    }

                    if (geo::squared_distance(member_i.point, member_j.point) > dcut2) {
                        continue;
                    }

                    *out++ = std::make_pair(member_i.index, member_j.index);
                }
            }
        }

        // locate_bin returns the hash index for a point.
        inline index_type locate_bin(point_type point) const
        {
            // Floating-point to *unsigned* integer conversion is costly.
            auto const to_index_type = [](scalar_type x) {
                return static_cast<index_type>(static_cast<std::ptrdiff_t>(x));
            };

            auto const freq = 1 / dcut_;
            auto const x = to_index_type(std::nearbyint(freq * point[0]));
            auto const y = to_index_type(std::nearbyint(freq * point[1]));
            auto const z = to_index_type(std::nearbyint(freq * point[2]));

            return linear_hash(x, y, z);
        }

        // linear_hash returns the index into the hash table bins_ for given
        // three dimensional bin index (x,y,z).
        inline index_type linear_hash(index_type x, index_type y, index_type z) const
        {
            return (x * x_stride + y * y_stride + z * z_stride) % bins_.size();
        }

    private:
        scalar_type dcut_;
        std::vector<hash_bin> bins_;
        std::vector<index_type> hash_deltas_;
    };
}}


//------------------------------------------------------------------------------
// basic types
//------------------------------------------------------------------------------

#include <cstddef>


namespace sim
{
    enum { dimension = 3 };

    using index = std::size_t;
    using scalar = double;

    using vector = geo::vector<scalar, dimension>;
    using point = geo::point<scalar, dimension>;

    using geo::dot;
    using geo::norm;
    using geo::squared_norm;
    using geo::cross;
    using geo::normalize;
    using geo::squared_distance;
    using geo::distance;
}


//------------------------------------------------------------------------------
// forcefield
//------------------------------------------------------------------------------

namespace sim
{
    class system;

    // forcefield is a base class of mechanical force fields.
    class forcefield
    {
    public:
        virtual ~forcefield() = default;

        // compute_energy computes potential energy of the system.
        virtual sim::scalar compute_energy(sim::system const& system) = 0;

        // compute_force computes force acting on the particles in the system
        // and adds calculated force vectors into force_array.
        virtual void compute_force(
            sim::system const& system,
            sim::array_view<sim::vector> force_array
        ) = 0;
    };
}


//------------------------------------------------------------------------------
// sum_forcefield
//------------------------------------------------------------------------------

#include <memory>
#include <vector>


namespace sim
{
    namespace detail
    {
        // sum_forcefield composes zero or more child forcefield instances into
        // a single forcefield. This class is used by sim::system.
        class sum_forcefield : public virtual sim::forcefield
        {
        public:
            // add_forcefield adds a forcefield as a child.
            void add_forcefield(std::shared_ptr<sim::forcefield> ff)
            {
                forcefields_.push_back(ff);
            }

            // compute_energy computes the sum of potential energy values
            // computed by child forcefield instances.
            sim::scalar compute_energy(sim::system const& system) override
            {
                sim::scalar sum = 0;
                for (auto& ff : forcefields_) {
                    sum += ff->compute_energy(system);
                }
                return sum;
            }

            // compute_force computes the sum of force vectors computed by child
            // forcefield instances.
            void compute_force(
                sim::system const& system,
                sim::array_view<sim::vector> force_array
            ) override
            {
                for (auto& ff : forcefields_) {
                    ff->compute_force(system, force_array);
                }
            }

        private:
            std::vector<std::shared_ptr<sim::forcefield>> forcefields_;
        };
    }
}


//------------------------------------------------------------------------------
// attributes
//------------------------------------------------------------------------------

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>


namespace sim
{
    template<typename T>
    struct basic_attribute_traits
    {
        using value_type = T;
        static inline constexpr value_type default_value() { return value_type(); }
    };

    template<typename T>
    struct attribute_traits : basic_attribute_traits<T>
    {
    };

    namespace detail
    {
        // attribute_base is the base class of type-erased arrays.
        class attribute_base
        {
        public:
            virtual ~attribute_base() = default;

            // resize resizes the internal storage to n-items length.
            virtual void resize(sim::index n) = 0;
        };

        // attribute stores an array of values identified by type T.
        template<typename T>
        class attribute : public attribute_base
        {
        public:
            using value_type = typename sim::attribute_traits<T>::value_type;

            void resize(sim::index n) override
            {
                values_.resize(n, sim::attribute_traits<T>::default_value());
            }

            inline sim::array_view<value_type> view() noexcept
            {
                return values_;
            }

        private:
            std::vector<value_type> values_;
        };

        // attribute_table is a set of columnar arrays with the same lengths.
        // Each column is keyed with a static type.
        class attribute_table
        {
        public:
            // size returns the common length of held arrays.
            inline sim::index size() const noexcept
            {
                return size_;
            }

            // resize changes the size of all held arrays.
            void resize(sim::index n)
            {
                for (auto& key_column : attrs_) {
                    key_column.second->resize(n);
                }
                size_ = n;
            }

            // make_attribute creates a column keyed with type T if it does not
            // exist yet.
            template<typename T>
            sim::array_view<typename sim::attribute_traits<T>::value_type> make_attribute()
            {
                std::type_index const key = typeid(T);

                if (attrs_.find(key) == attrs_.end()) {
                    auto attr = std::make_unique<detail::attribute<T>>();
                    attr->resize(size_);
                    attrs_.emplace(key, std::move(attr));
                }

                return view_attribute<T>();
            }

            // view_attribute
            template<typename T>
            inline sim::array_view<typename sim::attribute_traits<T>::value_type>
            view_attribute()
            {
                return static_cast<detail::attribute<T>&>(*attrs_.at(typeid(T))).view();
            }

            // view_attribute
            template<typename T>
            inline sim::array_view<typename sim::attribute_traits<T>::value_type const>
            view_attribute() const
            {
                return static_cast<detail::attribute<T>&>(*attrs_.at(typeid(T))).view();
            }

        private:
            std::unordered_map<std::type_index, std::unique_ptr<detail::attribute_base>> attrs_;
            sim::index size_ = 0;
        };
    }
}


//------------------------------------------------------------------------------
// system
//------------------------------------------------------------------------------

#include <memory>
#include <stdexcept>


namespace sim
{
    // mass_attribute is an attribute tag for particle mass.
    struct mass_attribute {};

    template<>
    struct attribute_traits<sim::mass_attribute> : basic_attribute_traits<sim::scalar>
    {
        static inline constexpr value_type default_value() { return 1; }
    };

    // position_attribute is an attribute tag for particle position.
    struct position_attribute {};

    template<>
    struct attribute_traits<sim::position_attribute> : basic_attribute_traits<sim::point> {};

    // velocity_attribute is an attribute tag for particle velocity.
    struct velocity_attribute {};

    template<>
    struct attribute_traits<sim::velocity_attribute> : basic_attribute_traits<sim::vector> {};

    // system holds particles and associated forcefield.
    class system
    {
    public:
        system()
        {
            attr_table_.make_attribute<sim::mass_attribute>();
            attr_table_.make_attribute<sim::position_attribute>();
            attr_table_.make_attribute<sim::velocity_attribute>();
        }

        // particle_count returns the number of particles in the system.
        inline sim::index particle_count() const noexcept
        {
            return attr_table_.size();
        }

        // add_particle adds a particle into the system.
        void add_particle()
        {
            attr_table_.resize(attr_table_.size() + 1);
        }

        // add_forcefield adds a forcefield into the system.
        void add_forcefield(std::shared_ptr<sim::forcefield> forcefield)
        {
            forcefield_.add_forcefield(forcefield);
        }

        // make_attribute ensures the existence of particle attribute T. Returns
        // a mutable view of attribtue values.
        template<typename T>
        sim::array_view<typename sim::attribute_traits<T>::value_type> make_attribute()
        {
            return attr_table_.make_attribute<T>();
        }

        // view_attribute returns a mutable view of attribute values.
        template<typename T>
        inline sim::array_view<typename sim::attribute_traits<T>::value_type> view_attribute()
        {
            return attr_table_.view_attribute<T>();
        }

        // view_attribute returns a read-only view of attribute values.
        template<typename T>
        inline sim::array_view<typename sim::attribute_traits<T>::value_type const>
        view_attribute() const
        {
            return attr_table_.view_attribute<T>();
        }

        // view_mass_array returns a mutable view of particle mass.
        inline sim::array_view<sim::scalar> view_mass_array() noexcept
        {
            return attr_table_.view_attribute<sim::mass_attribute>();
        }

        // view_mass_array returns a read-only view of particle mass.
        inline sim::array_view<sim::scalar const> view_mass_array() const noexcept
        {
            return attr_table_.view_attribute<sim::mass_attribute>();
        }

        // view_position_array returns a mutable view of particle position.
        inline sim::array_view<sim::point> view_position_array() noexcept
        {
            return attr_table_.view_attribute<sim::position_attribute>();
        }

        // view_position_array returns a read-only view of particle position.
        inline sim::array_view<sim::point const> view_position_array() const noexcept
        {
            return attr_table_.view_attribute<sim::position_attribute>();
        }

        // view_velocity_array returns a mutable view of particle velocity.
        inline sim::array_view<sim::vector> view_velocity_array() noexcept
        {
            return attr_table_.view_attribute<sim::velocity_attribute>();
        }

        // view_velocity_array returns a read-only view of particle velocity.
        inline sim::array_view<sim::vector const> view_velocity_array() const noexcept
        {
            return attr_table_.view_attribute<sim::velocity_attribute>();
        }

        // compute_potential_energy computes the total potential energy of the
        // particles in the system.
        sim::scalar compute_potential_energy()
        {
            return forcefield_.compute_energy(*this);
        }

        // compute_force computes the force acting on all particles.
        void compute_force(sim::array_view<sim::vector> force_array)
        {
            if (force_array.size() != particle_count()) {
                throw std::logic_error(
                    "sim::system::compute_force - "
                    "size of force_array is not equal to particle_count"
                );
            }

            for (auto& force : force_array) {
                force = {}; // Zero clear
            }

            forcefield_.compute_force(*this, force_array);
        }

    private:
        detail::attribute_table attr_table_;
        detail::sum_forcefield forcefield_;
    };
}


//------------------------------------------------------------------------------
// composite_forcefield
//------------------------------------------------------------------------------

namespace sim
{
    // composite_forcefield is a static composition of forcefield classes.
    template<typename... Components>
    class composite_forcefield : public Components..., virtual sim::forcefield
    {
    public:
        composite_forcefield() = default;

        template<typename... Inits>
        composite_forcefield(Inits const&... inits)
            : Inits(inits)...
        {
        }

        // compute_energy computes the sum of energy values computed by the
        // components.
        sim::scalar compute_energy(sim::system const& system) override
        {
            sim::scalar sum = 0;
            for (auto energy : {Components::compute_energy(system)...}) {
                sum += energy;
            }
            return sum;
        }

        // compute_force computes the sum of force vectors computed by the
        // components.
        void compute_force(sim::system const& system, sim::array_view<sim::vector> forces) override
        {
            int dummy[] = {
                0,
                (Components::compute_force(system, forces), 0)...
            };
            (void) dummy;
        }
    };
}


//------------------------------------------------------------------------------
// sequential_pair_forcefield
//------------------------------------------------------------------------------

#include <iterator>
#include <vector>


namespace sim
{
    template<typename Derived>
    class sequential_pair_forcefield : public virtual sim::forcefield
    {
    public:
        void add_segment(sim::index first, sim::index last)
        {
            segments_.emplace_back(first, last);
        }

        sim::scalar compute_energy(sim::system const& system) override
        {
            auto const positions = system.view_position_array();

            sim::scalar sum = 0;

            for (auto const& segment : segments_) {
                for (sim::index i = segment.first; i < segment.second; i++) {
                    auto const potential = derived().sequential_pair_potential(system, i, i + 1);
                    auto const r_ij = positions[i] - positions[i + 1];

                    sum += potential.evaluate_energy(r_ij);
                }
            }

            return sum;
        }

        void compute_force(sim::system const& system, sim::array_view<sim::vector> forces) override
        {
            auto const positions = system.view_position_array();

            for (auto const& segment : segments_) {
                for (sim::index i = segment.first; i < segment.second; i++) {
                    auto const potential = derived().sequential_pair_potential(system, i, i + 1);
                    auto const r_ij = positions[i] - positions[i + 1];
                    auto const force = potential.evaluate_force(r_ij);

                    forces[i] += force;
                    forces[i + 1] -= force;
                }
            }
        }

    private:
        Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        std::vector<std::pair<sim::index, sim::index>> segments_;
    };
}


//------------------------------------------------------------------------------
// potentials
//------------------------------------------------------------------------------

namespace sim
{
    struct harmonic_potential
    {
        sim::scalar spring_constant = 1;

        inline sim::scalar evaluate_energy(sim::vector r) const
        {
            return 0.5 * spring_constant * r.squared_norm();
        }

        inline sim::vector evaluate_force(sim::vector r) const
        {
            return -spring_constant * r;
        }
    };

    struct softcore_lennard_jones_potential
    {
        sim::scalar epsilon = 1;
        sim::scalar sigma = 1;
        sim::scalar softness = 1;

        inline sim::scalar evaluate_energy(sim::vector r) const
        {
            auto const r2 = r.squared_norm() / (sigma * sigma);
            auto const r6 = r2 * r2 * r2;

            auto const u = 1 / (softness + r6);
            auto const w = u - r6 * u;

            return epsilon * w * w;
        }

        inline sim::vector evaluate_force(sim::vector r) const
        {
            auto const s2 = 1 / (sigma * sigma);
            auto const r2 = r.squared_norm() * s2;
            auto const r4 = r2 * r2;
            auto const r6 = r4 * r2;

            auto const u = 1 / (softness + r6);
            auto const v = softness * u + u;

            return 12 * epsilon * s2 * r4 * v * (v * u - u) * r;
        }
    };

    // u(r) = e (1 - k r^2)^4
    // F(r) = 8ke (1 - k r^2)^3 r
    struct softcore_potential
    {
        sim::scalar repulsive_energy = 1;
        sim::scalar squared_cutoff_distance = 1;

        inline sim::scalar evaluate_energy(sim::vector r) const
        {
            auto const r2 = squared_cutoff_distance * r.squared_norm();
            auto const u1 = 1 - r2;

            if (u1 < 0) {
                return 0;
            }

            auto const u2 = u1 * u1;
            auto const u4 = u2 * u2;

            return repulsive_energy * u4;
        }

        inline sim::vector evaluate_force(sim::vector r) const
        {
            auto const r2 = squared_cutoff_distance * r.squared_norm();
            auto const u1 = 1 - r2;

            if (u1 < 0) {
                return sim::vector {};
            }

            auto const u3 = u1 * u1 * u1;
            auto const coef = 8 * squared_cutoff_distance * repulsive_energy;

            return coef * u3 * r;
        }
    };
}

#endif
