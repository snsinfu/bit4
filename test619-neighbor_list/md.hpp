#ifndef INCLUDED_MD_HPP
#define INCLUDED_MD_HPP

//------------------------------------------------------------------------------
// vendor - array_view
//------------------------------------------------------------------------------

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace md
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
        constexpr auto data(T (&arr)[N]) noexcept -> T*
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
        constexpr auto size(T (&)[N]) noexcept -> std::size_t
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
        constexpr array_view(pointer p, size_type n) : data_ {p}, size_ {n}
        {
        }

        // array_view is implicitly convertible from a contiguous container like
        // std::vector.
        template<
            typename Container,
            typename P = decltype(array_view_detail::data(std::declval<Container&>())),
            typename S = decltype(array_view_detail::size(std::declval<Container&>())),
            typename = typename std::enable_if<
                std::is_convertible<typename std::remove_pointer<P>::type (*)[], T (*)[]>::value,
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
            return reverse_iterator {end()};
        }

        // rend returns a reverse iterator to the reverse end.
        reverse_iterator rend() const noexcept
        {
            return reverse_iterator {begin()};
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
// vendor - points
//------------------------------------------------------------------------------

#include <cmath>
#include <cstddef>
#include <istream>
#include <ostream>


namespace md
{
    namespace point_detail
    {
        // coordinates hold three-dimensional cartesian coordinate values.
        struct coordinates
        {
            double x = 0;
            double y = 0;
            double z = 0;

            // Default constructor initializes coordinates to zero.
            coordinates() = default;

            // Constructor initializes coordinates to specified values.
            inline coordinates(double x_init, double y_init, double z_init)
                : x{x_init}, y{y_init}, z{z_init}
            {
            }

            // Index 0, 1 and 2 correspond to x, y and z, respectively.
            inline double& operator[](std::size_t idx)
            {
                double* coords[] = {
                    &x,
                    &y,
                    &z
                };
                return *coords[idx];
            }

            // Index 0, 1 and 2 correspond to x, y and z, respectively.
            inline double const& operator[](std::size_t idx) const
            {
                double const* coords[] = {
                    &x,
                    &y,
                    &z
                };
                return *coords[idx];
            }
        };

        // ostream operator formats coords as space-delimited floating-point
        // coordinate values.
        template<typename Char, typename Tr>
        std::basic_ostream<Char, Tr>& operator<<(
            std::basic_ostream<Char, Tr>& os,
            coordinates const& coords
        )
        {
            using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

            if (sentry_type sentry {os}) {
                Char const delim = os.widen(' ');
                os << coords.x << delim << coords.y << delim << coords.z;
            }

            return os;
        }

        // istream operator parses space-delimited floating-point coordinate
        // values into coords.
        template<typename Char, typename Tr>
        std::basic_istream<Char, Tr>& operator>>(
            std::basic_istream<Char, Tr>& is,
            coordinates& coords
        )
        {
            using sentry_type = typename std::basic_istream<Char, Tr>::sentry;

            if (sentry_type sentry {is}) {
                is >> coords.x >> coords.y >> coords.z;
            }

            return is;
        }
    }

    // vector is a three-dimensional vector with cartesian coordinates.
    struct vector : point_detail::coordinates
    {
        using point_detail::coordinates::coordinates;

        // Component-wise addition.
        inline vector& operator+=(vector const& other) noexcept
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        // Component-wise subtraction.
        inline vector& operator-=(vector const& other) noexcept
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Scalar multiplication is broadcast to all components.
        inline vector& operator*=(double mult) noexcept
        {
            x *= mult;
            y *= mult;
            z *= mult;
            return *this;
        }

        // Scalar division is broadcast to all components.
        inline vector& operator/=(double divisor)
        {
            return *this *= 1 / divisor;
        }

        // dot returns the dot product of this vector and other.
        inline double dot(vector const& other) const noexcept
        {
            return (x * other.x + y * other.y + z * other.z);
        }

        // cross returns the cross product of this vector and rhs.
        inline vector cross(vector const& rhs) const noexcept
        {
            return vector {
                y * rhs.z - z * rhs.y,
                z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x
            };
        }

        // norm returns the Euclidean norm of this vector.
        inline double norm() const noexcept
        {
            return std::sqrt(squared_norm());
        }

        // squared_norm returns the squared Euclidean norm of this vector.
        inline double squared_norm() const noexcept
        {
            return dot(*this);
        }

        // normalize returns the unit vector that is parallel to this vector.
        // Behavior is undefined if this vector is zero.
        inline vector normalize() const
        {
            return vector{*this} /= norm();
        }
    };

    // Unary no-op `+vec` returns a copy of vec.
    inline vector operator+(vector const& vec) noexcept
    {
        return vec;
    }

    // Unary negation `-vec` returns an anti-parallel copy of vec.
    inline vector operator-(vector const& vec) noexcept
    {
        return {-vec.x, -vec.y, -vec.z};
    }

    // Sum `lhs + rhs` returns a vector with component-wise sum of lhs and rhs.
    inline vector operator+(vector const& lhs, vector const& rhs) noexcept
    {
        return vector{lhs} += rhs;
    }

    // Difference `lhs - rhs` returns a vector with component-wise difference of
    // lhs and rhs.
    inline vector operator-(vector const& lhs, vector const& rhs) noexcept
    {
        return vector{lhs} -= rhs;
    }

    // Multiplication `vec * mult` returns vec with each component scaled by
    // mult.
    inline vector operator*(vector const& vec, double mult) noexcept
    {
        return vector{vec} *= mult;
    }

    // Multiplication `mult * vec` returns vec with each component scaled by
    // mult.
    inline vector operator*(double mult, vector const& vec) noexcept
    {
        return vector{vec} *= mult;
    }

    // Division `vec / divisor` returns vec with each component scaled by
    // 1/divisor.
    inline vector operator/(vector const& vec, double divisor)
    {
        return vector{vec} /= divisor;
    }

    // dot returns the dot product of lhs and rhs.
    inline double dot(vector const& lhs, vector const& rhs) noexcept
    {
        return lhs.dot(rhs);
    }

    // cross returns the cross product of lhs and rhs.
    inline vector cross(vector const& lhs, vector const& rhs) noexcept
    {
        return lhs.cross(rhs);
    }

    // norm returns the Euclidean norm of vec.
    inline double norm(vector const& vec) noexcept
    {
        return vec.norm();
    }

    // squared_norm returns the squares Euclidean norm of vec.
    inline double squared_norm(vector const& vec) noexcept
    {
        return vec.squared_norm();
    }

    // normalize returns the unit vector that is parallel to vec.
    inline vector normalize(vector const& vec) noexcept
    {
        return vec.normalize();
    }

    // point is a cartesian representation of a point in the three-dimensional
    // Euclidean space.
    struct point : point_detail::coordinates
    {
        using point_detail::coordinates::coordinates;
        using vector_t = struct vector;

        // vector returns the coordinate vector of this point.
        inline vector_t vector() const noexcept
        {
            return vector_t {x, y, z};
        }

        // Adding a vector translates the point.
        inline point& operator+=(vector_t const& disp) noexcept
        {
            x += disp.x;
            y += disp.y;
            z += disp.z;
            return *this;
        }

        // Subtracting a vector translates the point.
        inline point& operator-=(vector_t const& disp) noexcept
        {
            x -= disp.x;
            y -= disp.y;
            z -= disp.z;
            return *this;
        }

        // distance returns the Euclidean distance between this point and other.
        inline double distance(point const& other) const noexcept
        {
            return (vector() - other.vector()).norm();
        }

        // squared_distance returns the squared Euclidean distance between this
        // point and other.
        inline double squared_distance(point const& other) const noexcept
        {
            return (vector() - other.vector()).squared_norm();
        }
    };

    // Sum `pt + disp` returns the point pt translated by disp.
    inline point operator+(point const& pt, vector const& disp) noexcept
    {
        return point {pt} += disp;
    }

    // Difference `pt - disp` returns the point pt translated by -disp.
    inline point operator-(point const& pt, vector const& disp) noexcept
    {
        return point {pt} -= disp;
    }

    // Difference `pa - pb` returns the displacement vector from pb towards pa.
    inline vector operator-(point const& pa, point const& pb) noexcept
    {
        return pa.vector() - pb.vector();
    }

    // distance returns the Euclidean distance between pa and pb.
    inline double distance(point const& pa, point const& pb) noexcept
    {
        return pa.distance(pb);
    }

    // squared_distance returns the squared Euclidean distance between pa and
    // pb.
    inline double squared_distance(point const& pa, point const& pb) noexcept
    {
        return pa.squared_distance(pb);
    }
}


//------------------------------------------------------------------------------
// basic types
//------------------------------------------------------------------------------

#include <cstddef>


namespace md
{
    // index is the integral type used to index arrays.
    using index = std::size_t;

    // scalar is the floating-point type of choice.
    using scalar = double;

    // dimension is the spatial dimension, which is 3.
    enum { dimension = 3 };
}


//------------------------------------------------------------------------------
// neighbor_searcher and neighbor_list
//------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>


namespace md
{
    // neighbor_searcher is a data structure for searching pairs of neighboring
    // points in O(N) complexity.
    class neighbor_searcher
    {
        // Prime coefficients used for coordinate hashing.
        static constexpr md::index x_stride = 73856093;
        static constexpr md::index y_stride = 19349669;
        static constexpr md::index z_stride = 83492791;

    public:
        // Constructor takes the cutoff distance and the number of bins used for
        // hashing points.
        //
        // The optimal number of bins depends on the actual distribution of
        // points. A rule-of-thumb number for a dense system is the number of
        // points divided by 20.
        neighbor_searcher(md::scalar dcut, md::index bins)
            : dcut_(dcut), bins_(bins)
        {
            // Pre-compute the hash index differences between adjacent bins. We
            // use a linear hash function so the differences are the same (i.e.,
            // reusable) for all bins.

            md::index const index_deltas[] = {bins - 1, bins, bins + 1};

            for (auto dx : index_deltas) {
                for (auto dy : index_deltas) {
                    for (auto dz : index_deltas) {
                        nearby_deltas_.push_back(linear_hash(dx, dy, dz));
                    }
                }
            }

            std::sort(nearby_deltas_.begin(), nearby_deltas_.end());

            nearby_deltas_.erase(
                std::unique(nearby_deltas_.begin(), nearby_deltas_.end()),
                nearby_deltas_.end()
            );
        }

        // set_points clears internal hash table and assigns given points to the
        // hash table.
        void set_points(md::array_view<md::point const> points)
        {
            for (hash_bin& bin : bins_) {
                bin.members.clear();
            }

            for (md::index idx = 0; idx < points.size(); idx++) {
                md::point const pt = points[idx];
                hash_bin& bin = bins_[locate_bin(pt)];
                bin.members.push_back({idx, pt});
            }
        }

        // search searches for neighbor pairs in the hash table and outputs
        // index pairs to given output iterator. Each index pair (i,j) satisfies
        // i < j and no duplicate pairs are output.
        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            md::index const bin_count = bins_.size();

            for (md::index center = 0; center < bin_count; center++) {
                for (md::index const delta : nearby_deltas_) {
                    hash_bin const& center_bin = bins_[center];
                    hash_bin const& nearby_bin = bins_[(center + delta) % bin_count];

                    search_among(center_bin, nearby_bin, out);
                }
            }
        }

    private:
        struct hash_bin
        {
            struct member
            {
                md::index index;
                md::point point;
            };
            std::vector<member> members;
        };

        // search_among searches for pairs of neighbor points in given bins. It
        // outputs index pairs to out. Each index pair (i,j) satisfies i < j
        // and no duplicate pairs are output.
        template<typename OutputIterator>
        inline void search_among(
            hash_bin const& bin_a,
            hash_bin const& bin_b,
            OutputIterator& out
        ) const
        {
            md::scalar const dcut2 = dcut_ * dcut_;

            for (auto member_i : bin_a.members) {
                for (auto member_j : bin_b.members) {
                    if (member_i.index >= member_j.index) {
                        continue;
                    }

                    if (md::squared_distance(member_i.point, member_j.point) > dcut2) {
                        continue;
                    }

                    *out++ = std::make_pair(member_i.index, member_j.index);
                }
            }
        }

    public:
        // locate_bin returns the hash index for a point.
        inline md::index locate_bin(md::point pt) const
        {
            // Floating-point to *unsigned* integer conversion is costly.
            auto const to_index_type = [](md::scalar x) {
                return static_cast<md::index>(static_cast<std::ptrdiff_t>(x));
            };

            auto const freq = 1 / dcut_;
            auto const x = to_index_type(std::nearbyint(freq * pt.x));
            auto const y = to_index_type(std::nearbyint(freq * pt.y));
            auto const z = to_index_type(std::nearbyint(freq * pt.z));

            return linear_hash(x, y, z);
        }

        // linear_hash returns the index into the hash table bins_ for given
        // three dimensional bin index (x,y,z).
        inline md::index linear_hash(md::index x, md::index y, md::index z) const
        {
            return (x * x_stride + y * y_stride + z * z_stride) % bins_.size();
        }

    private:
        md::scalar dcut_;
        std::vector<hash_bin> bins_;
        std::vector<md::index> nearby_deltas_;
    };


    // neighbor_list is a data structure for keeping track of neighbor pairs in
    // a slowly moving particle system.
    class neighbor_list
    {
        static constexpr md::index min_bin_count = 10;

    public:
        using const_iterator = std::vector<std::pair<md::index, md::index>>::const_iterator;

        neighbor_list(md::scalar skin_factor, md::index mean_occupancy)
            : skin_factor_{skin_factor}
            , mean_occupancy_{mean_occupancy}
        {
        }

        void update(md::array_view<md::point const> points, md::scalar dcut)
        {
            if (!check_consistency(points, dcut)) {
                rebuild(points, dcut);
            }
        }

        const_iterator begin() const
        {
            return pairs_.begin();
        }

        const_iterator end() const
        {
            return pairs_.end();
        }

    private:
        bool check_consistency(md::array_view<md::point const> points, md::scalar dcut) const
        {
            if (points.size() != cached_points_.size()) {
                return false;
            }

            // False negatives (unlisted point pairs that lie actually within
            // dcut) won't arise if the displacement from previous rebuild is
            // less than or equal to this threshold.
            auto const threshold = (verlet_radius_ - dcut) / 2;
            if (threshold <= 0) {
                return false;
            }

            for (md::index i = 0; i < points.size(); i++) {
                auto const disp_2 = md::squared_distance(points[i], cached_points_[i]);
                if (disp_2 > threshold * threshold) {
                    return false;
                }
            }

            return true;
        }

        void rebuild(md::array_view<md::point const> points, md::scalar dcut)
        {
            auto const verlet_radius = dcut * skin_factor_;
            auto const bin_count = std::max(points.size() / mean_occupancy_, min_bin_count);

            pairs_.clear();
            cached_points_.assign(points.begin(), points.end());
            verlet_radius_ = verlet_radius;

            md::neighbor_searcher searcher{verlet_radius, bin_count};
            searcher.set_points(points);
            searcher.search(std::back_inserter(pairs_));
        }

    private:
        md::scalar skin_factor_;
        md::index mean_occupancy_;
        std::vector<std::pair<md::index, md::index>> pairs_;
        std::vector<md::point> cached_points_;
        md::scalar verlet_radius_ = 0;
    };
}

#endif
