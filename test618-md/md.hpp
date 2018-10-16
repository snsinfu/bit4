#ifndef INCLUDED_MD_HPP
#define INCLUDED_MD_HPP

// md {
//   class array_view
//   class vector
//   class point
//
//   class neighbor_searcher
//   class neighbor_list
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

            for (md::index idx = 0; idx < points.size(); ++idx) {
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

            // Cache optimization
            md::index const block_size = 4;
            md::index const block_count = bin_count / block_size;
            md::index const residual_start = block_size * block_count;

            for (md::index block = 0; block < residual_start; block += block_size) {
                for (md::index const delta : nearby_deltas_) {
                    for (md::index offset = 0; offset < block_size; offset++) {
                        md::index const center = block + offset;
                        hash_bin const& center_bin = bins_[center];
                        hash_bin const& nearby_bin = bins_[(center + delta) % bin_count];

                        search_among(center_bin, nearby_bin, out);
                    }
                }
            }

            for (md::index center = residual_start; center < bin_count; center++) {
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
            auto const dcut2 = dcut_ * dcut_;

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

        // locate_bin returns the hash index for a point.
        inline md::index locate_bin(md::point pt) const
        {
            // Wraparound of negative value breaks the linearity of our hash
            // function. Avoid that by offsetting.
            auto const offset = md::scalar(1L << 30);

            auto const freq = 1 / dcut_;
            auto const x = md::index(offset + freq * pt.x);
            auto const y = md::index(offset + freq * pt.y);
            auto const z = md::index(offset + freq * pt.z);

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
    public:
        using iterator = std::vector<std::pair<md::index, md::index>>::const_iterator;

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

        iterator begin() const
        {
            return pairs_.begin();
        }

        iterator end() const
        {
            return pairs_.end();
        }

    private:
        bool check_consistency(md::array_view<md::point const> points, md::scalar dcut) const
        {
            if (points.size() != cached_points_.size()) {
                return false;
            }

            // False negatives (unlisted point pairs that fall actually within
            // dcut) won't arise if the displacement from previous rebuild is
            // less than or equal to this threshold (by math).
            auto const threshold = (verlet_radius_ - dcut) / 2;

            if (threshold <= 0) {
                return false;
            }

            for (md::index i = 0; i < points.size(); i++) {
                if (md::squared_distance(points[i], cached_points_[i]) > threshold * threshold) {
                    return false;
                }
            }

            return true;
        }

        void rebuild(md::array_view<md::point const> points, md::scalar dcut)
        {
            auto const verlet_radius = dcut * skin_factor_;
            auto const bin_count = points.size() / mean_occupancy_;

            pairs_.clear();
            cached_points_.assign(points.begin(), points.end());
            verlet_radius_ = verlet_radius;

            md::neighbor_searcher searcher{verlet_radius, bin_count};
            searcher.set_points(cached_points_);
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


//------------------------------------------------------------------------------
// forcefield
//------------------------------------------------------------------------------

namespace md
{
    class system;

    // forcefield is an abstract base class for all force field implementations.
    class forcefield
    {
    public:
        virtual ~forcefield() = default;

        // compute_energy computes potential energy of the system.
        virtual md::scalar compute_energy(md::system const& system) = 0;

        // compute_force computes force acting on the particles in the system
        // and adds calculated force vectors into forces.
        virtual void compute_force(
            md::system const& system,
            md::array_view<md::vector> forces
        ) = 0;
    };
}


//------------------------------------------------------------------------------
// sum_forcefield
//------------------------------------------------------------------------------

#include <memory>
#include <vector>


namespace md
{
    namespace detail
    {
        // sum_forcefield composes zero or more child forcefield instances into
        // a single forcefield. This class is used by md::system.
        class sum_forcefield : public virtual md::forcefield
        {
        public:
            // add_forcefield adds a forcefield as a child.
            void add_forcefield(std::shared_ptr<md::forcefield> ff)
            {
                forcefields_.push_back(ff);
            }

            // compute_energy computes the sum of potential energy values
            // computed by child forcefield instances.
            md::scalar compute_energy(md::system const& system) override
            {
                md::scalar sum = 0;
                for (auto& ff : forcefields_) {
                    sum += ff->compute_energy(system);
                }
                return sum;
            }

            // compute_force computes the sum of force vectors computed by child
            // forcefield instances.
            void compute_force(md::system const& system, md::array_view<md::vector> forces) override
            {
                for (auto& ff : forcefields_) {
                    ff->compute_force(system, forces);
                }
            }

        private:
            std::vector<std::shared_ptr<md::forcefield>> forcefields_;
        };
    }
}


//------------------------------------------------------------------------------
// attributes system
//------------------------------------------------------------------------------

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>


namespace md
{
    template<typename T>
    struct basic_attribute_traits
    {
        using value_type = T;

        static inline constexpr value_type default_value()
        {
            return value_type();
        }
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
            virtual void resize(md::index n) = 0;
        };

        // attribute stores an array of values identified by type T.
        template<typename T>
        class attribute : public attribute_base
        {
        public:
            using value_type = typename md::attribute_traits<T>::value_type;

            void resize(md::index n) override
            {
                values_.resize(n, md::attribute_traits<T>::default_value());
            }

            inline md::array_view<value_type> view() noexcept
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
            inline md::index size() const noexcept
            {
                return size_;
            }

            // resize changes the size of all held arrays.
            void resize(md::index n)
            {
                for (auto& key_column : attrs_) {
                    key_column.second->resize(n);
                }
                size_ = n;
            }

            // require_attribute creates a column keyed with type T if it does
            // not exist.
            template<typename T>
            void require_attribute()
            {
                std::type_index const key = typeid(T);

                if (attrs_.find(key) == attrs_.end()) {
                    auto attr = std::make_unique<detail::attribute<T>>();
                    attr->resize(size_);
                    attrs_.emplace(key, std::move(attr));
                }
            }

            // view_attribute
            template<typename T>
            inline md::array_view<typename md::attribute_traits<T>::value_type> view_attribute()
            {
                return static_cast<detail::attribute<T>&>(*attrs_.at(typeid(T))).view();
            }

            // view_attribute
            template<typename T>
            inline md::array_view<typename md::attribute_traits<T>::value_type const>
            view_attribute() const
            {
                return static_cast<detail::attribute<T>&>(*attrs_.at(typeid(T))).view();
            }

        private:
            std::unordered_map<std::type_index, std::unique_ptr<detail::attribute_base>> attrs_;
            md::index size_ = 0;
        };
    }
}


//------------------------------------------------------------------------------
// system
//------------------------------------------------------------------------------

#include <memory>
#include <stdexcept>


namespace md
{
    // mass_attribute is an attribute tag for particle mass.
    struct mass_attribute
    {
    };

    template<>
    struct attribute_traits<md::mass_attribute> : basic_attribute_traits<md::scalar>
    {
        static inline constexpr md::scalar default_value()
        {
            return 1;
        }
    };

    // position_attribute is an attribute tag for particle position.
    struct position_attribute
    {
    };

    template<>
    struct attribute_traits<md::position_attribute> : basic_attribute_traits<md::point>
    {
    };

    // velocity_attribute is an attribute tag for particle velocity.
    struct velocity_attribute
    {
    };

    template<>
    struct attribute_traits<md::velocity_attribute> : basic_attribute_traits<md::vector>
    {
    };

    // mobility_attribute
    struct mobility_attribute
    {
    };

    template<>
    struct attribute_traits<md::mobility_attribute> : basic_attribute_traits<md::scalar>
    {
        static inline constexpr md::scalar default_value()
        {
            return 1;
        }
    };

    struct basic_particle_attributes
    {
        md::scalar mass = attribute_traits<md::mass_attribute>::default_value();
        md::point position = attribute_traits<md::position_attribute>::default_value();
        md::vector velocity = attribute_traits<md::velocity_attribute>::default_value();
        md::scalar mobility = attribute_traits<md::mobility_attribute>::default_value();
    };

    // system holds particles and associated forcefield.
    class system
    {
    public:
        system()
        {
            attr_table_.require_attribute<md::mass_attribute>();
            attr_table_.require_attribute<md::position_attribute>();
            attr_table_.require_attribute<md::velocity_attribute>();
            attr_table_.require_attribute<md::mobility_attribute>();
        }

        // particle_count returns the number of particles in the system.
        inline md::index particle_count() const noexcept
        {
            return attr_table_.size();
        }

        // add_particle adds a particle into the system.
        void add_particle(md::basic_particle_attributes attrs = {})
        {
            auto const idx = attr_table_.size();

            attr_table_.resize(attr_table_.size() + 1);

            mass_array()[idx] = attrs.mass;
            position_array()[idx] = attrs.position;
            velocity_array()[idx] = attrs.velocity;
            mobility_array()[idx] = attrs.mobility;
        }

        // add_forcefield adds a forcefield into the system.
        void add_forcefield(std::shared_ptr<md::forcefield> forcefield)
        {
            forcefield_.add_forcefield(forcefield);
        }

        // require_attribute ensures the existence of particle attribute T.
        template<typename T>
        void require_attribute()
        {
            attr_table_.require_attribute<T>();
        }

        // attribute returns a mutable view of attribute values.
        template<typename T>
        inline md::array_view<typename md::attribute_traits<T>::value_type> attribute()
        {
            return attr_table_.view_attribute<T>();
        }

        // attribute returns a read-only view of attribute values.
        template<typename T>
        inline md::array_view<typename md::attribute_traits<T>::value_type const> attribute() const
        {
            return attr_table_.view_attribute<T>();
        }

        // particle_masses returns a mutable view of particle mass.
        inline md::array_view<md::scalar> mass_array() noexcept
        {
            return attr_table_.view_attribute<md::mass_attribute>();
        }

        // view_mass_array returns a read-only view of particle mass.
        inline md::array_view<md::scalar const> mass_array() const noexcept
        {
            return attr_table_.view_attribute<md::mass_attribute>();
        }

        // position_array returns a mutable view of particle position.
        inline md::array_view<md::point> position_array() noexcept
        {
            return attr_table_.view_attribute<md::position_attribute>();
        }

        // position_array returns a read-only view of particle position.
        inline md::array_view<md::point const> position_array() const noexcept
        {
            return attr_table_.view_attribute<md::position_attribute>();
        }

        // velocity_array returns a mutable view of particle velocity.
        inline md::array_view<md::vector> velocity_array() noexcept
        {
            return attr_table_.view_attribute<md::velocity_attribute>();
        }

        // velocity_array returns a read-only view of particle velocity.
        inline md::array_view<md::vector const> velocity_array() const noexcept
        {
            return attr_table_.view_attribute<md::velocity_attribute>();
        }

        // mobility_array
        inline md::array_view<md::scalar> mobility_array() noexcept
        {
            return attr_table_.view_attribute<md::mobility_attribute>();
        }

        // mobility_array
        inline md::array_view<md::scalar const> mobility_array() const noexcept
        {
            return attr_table_.view_attribute<md::mobility_attribute>();
        }

        // compute_potential_energy computes the total potential energy of the
        // particles in the system.
        md::scalar compute_potential_energy()
        {
            return forcefield_.compute_energy(*this);
        }

        // compute_force computes the force acting on all particles.
        void compute_force(md::array_view<md::vector> forces)
        {
            if (forces.size() != particle_count()) {
                throw std::logic_error(
                    "md::system::compute_force - "
                    "size of forces is not equal to particle_count"
                );
            }

            std::fill(forces.begin(), forces.end(), md::vector {0, 0, 0});
            forcefield_.compute_force(*this, forces);
        }

    private:
        detail::attribute_table attr_table_;
        detail::sum_forcefield forcefield_;
    };
}


//------------------------------------------------------------------------------
// composite_forcefield
//------------------------------------------------------------------------------

namespace md
{
    // composite_forcefield is a static composition of forcefield classes.
    template<typename... Components>
    class composite_forcefield : public Components..., virtual md::forcefield
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
        md::scalar compute_energy(md::system const& system) override
        {
            md::scalar sum = 0;
            for (auto energy : {Components::compute_energy(system)...}) {
                sum += energy;
            }
            return sum;
        }

        // compute_force computes the sum of force vectors computed by the
        // components.
        void compute_force(md::system const& system, md::array_view<md::vector> forces) override
        {
            int dummy[] = {0, (Components::compute_force(system, forces), 0)...};
            (void) dummy;
        }
    };
}


//------------------------------------------------------------------------------
// sphere_surface_forcefield
//------------------------------------------------------------------------------

namespace md
{
    template<typename Derived>
    class sphere_surface_forcefield : public virtual md::forcefield
    {
    public:
        void set_radius(md::scalar radius)
        {
            radius_ = radius;
        }

        md::scalar compute_energy(md::system const& system) override
        {
            md::scalar sum = 0;
            auto const positions = system.position_array();

            for (md::index i = 0; i < system.particle_count(); i++) {
                auto const r = positions[i] - center_;
                auto const pot = derived().sphere_outward_potential(system, i);

                sum += pot.evaluate_energy(r);
            }

            return sum;
        }

        void compute_force(md::system const& system, md::array_view<md::vector> forces) override
        {
            auto const positions = system.position_array();

            for (md::index i = 0; i < system.particle_count(); i++) {
                auto const r = positions[i] - center_;
                auto const pot = derived().sphere_outward_potential(system, i);

                forces[i] += pot.evaluate_force(r);
            }
        }

    private:
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        md::point center_;
        md::scalar radius_;
    };

    template<typename Potential>
    void force_sphere_surface(md::system& sys, md::scalar radius, Potential pot)
    {
        class impl : public md::sphere_surface_forcefield<impl>
        {
        public:
            impl(md::scalar radius, Potential pot)
                : pot_{pot}
            {
                this->set_radius(radius);
            }

            inline Potential sphere_outward_potential(md::system const&, md::index) const
            {
                return pot_;
            }

        private:
            Potential pot_;
        };

        sys.add_forcefield(std::make_shared<impl>(radius, pot));
    }
}


//------------------------------------------------------------------------------
// neighbor_pair_forcefield
//------------------------------------------------------------------------------

namespace md
{
    template<typename Derived>
    class neighbor_pair_forcefield : public virtual md::forcefield
    {
        // FIXME: These are rule-of-thumb numbers.
        static constexpr md::scalar default_skin_factor = 1.3;
        static constexpr md::index default_mean_occupancy = 20;

    public:
        neighbor_pair_forcefield()
            : nlist_{default_skin_factor, default_mean_occupancy}
        {
        }

        md::scalar compute_energy(md::system const& system) override
        {
            md::scalar sum = 0;
            auto const positions = system.position_array();

            nlist_.update(positions, derived().neighbor_pair_cutoff(system));

            for (auto const pair : nlist_) {
                auto const i = pair.first;
                auto const j = pair.second;

                auto const r = positions[i] - positions[j];
                auto const pot = derived().neighbor_pair_potential(system, i, j);
                sum += pot.evaluate_energy(r);
            }

            return sum;
        }

        void compute_force(md::system const& system, md::array_view<md::vector> forces) override
        {
            auto const positions = system.position_array();

            nlist_.update(positions, derived().neighbor_pair_cutoff(system));

            for (auto const pair : nlist_) {
                auto const i = pair.first;
                auto const j = pair.second;

                auto const r = positions[i] - positions[j];
                auto const pot = derived().neighbor_pair_potential(system, i, j);
                auto const force = pot.evaluate_force(r);

                forces[i] += force;
                forces[j] -= force;
            }
        }

    private:
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        md::neighbor_list nlist_;
    };


    template<typename Potential>
    void force_neighbor_pairs(md::system& sys, md::scalar dcut, Potential pot)
    {
        class impl : public md::neighbor_pair_forcefield<impl>
        {
        public:
            impl(md::scalar dcut, Potential pot)
                : dcut_{dcut}, pot_{pot}
            {
            }

            inline md::scalar neighbor_pair_cutoff(md::system const&) const
            {
                return dcut_;
            }

            inline Potential neighbor_pair_potential(md::system const&, md::index, md::index) const
            {
                return pot_;
            }

        private:
            md::scalar dcut_;
            Potential pot_;
        };

        sys.add_forcefield(std::make_shared<impl>(dcut, pot));
    }
}


//------------------------------------------------------------------------------
// sequential_pair_forcefield
//------------------------------------------------------------------------------

#include <iterator>
#include <vector>


namespace md
{
    template<typename Derived>
    class sequential_pair_forcefield : public virtual md::forcefield
    {
    public:
        // FIXME: Too generic function name.
        void add_segment(md::index first, md::index last)
        {
            segments_.emplace_back(first, last);
        }

        md::scalar compute_energy(md::system const& system) override
        {
            auto const positions = system.position_array();

            md::scalar sum = 0;

            for (auto const& segment : segments_) {
                for (md::index i = segment.first; i < segment.second; i++) {
                    auto const potential = derived().sequential_pair_potential(system, i, i + 1);
                    auto const r = positions[i] - positions[i + 1];

                    sum += potential.evaluate_energy(r);
                }
            }

            return sum;
        }

        void compute_force(md::system const& system, md::array_view<md::vector> forces) override
        {
            auto const positions = system.position_array();

            for (auto const& segment : segments_) {
                for (md::index i = segment.first; i < segment.second; i++) {
                    auto const potential = derived().sequential_pair_potential(system, i, i + 1);
                    auto const r = positions[i] - positions[i + 1];
                    auto const force = potential.evaluate_force(r);

                    forces[i] += force;
                    forces[i + 1] -= force;
                }
            }
        }

    private:
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        std::vector<std::pair<md::index, md::index>> segments_;
    };
}


//------------------------------------------------------------------------------
// pairwise potentials
//------------------------------------------------------------------------------

namespace md
{
    //
    // u(r) = K/2 r^2
    // K: spring_constant
    //
    struct harmonic_potential
    {
        md::scalar spring_constant = 1;

        inline md::scalar evaluate_energy(md::vector r) const
        {
            return 0.5 * spring_constant * r.squared_norm();
        }

        inline md::vector evaluate_force(md::vector r) const
        {
            return -spring_constant * r;
        }
    };


    //
    // u(r) = K/2 (r - b)^2
    // K: spring_constant
    // b: spring_distance
    //
    struct spring_potential
    {
        md::scalar spring_constant = 1;
        md::scalar spring_distance = 1;

        inline md::scalar evaluate_energy(md::vector r) const
        {
            auto const d = r.norm();
            auto const u = spring_distance - d;

            return 0.5 * spring_constant * u * u;
        }

        inline md::vector evaluate_force(md::vector r) const
        {
            auto const d = r.norm();
            auto const u = spring_distance - d;

            return spring_constant * u / d * r;
        }
    };


    //
    // u(r) = e ( (s/r)^12 - 2 (e/r)^6 )
    //
    struct lennard_jones_potential
    {
        md::scalar epsilon = 1;
        md::scalar sigma = 1;

        inline md::scalar evaluate_energy(md::vector r) const
        {
            auto const u2 = sigma * sigma / r.squared_norm();
            auto const u6 = u2 * u2 * u2;
            auto const u12 = u6 * u6;

            return epsilon * (u12 - u6 - u6);
        }

        inline md::vector evaluate_force(md::vector r) const
        {
            auto const r_inv2 = 1 / r.squared_norm();
            auto const u2 = sigma * sigma * r_inv2;
            auto const u6 = u2 * u2 * u2;
            auto const u12 = u6 * u6;

            return 12 * epsilon * (u12 - u6) * r_inv2 * r;
        }
    };


    //
    // u(r) = e ( (k+1)/(k+(r/s)^6) - 1 )^2
    // e: epsilon
    // s: sigma
    // k: softness
    //
    struct softcore_lennard_jones_potential
    {
        md::scalar epsilon = 1;
        md::scalar sigma = 1;
        md::scalar softness = 1;

        inline md::scalar evaluate_energy(md::vector r) const
        {
            auto const r2 = r.squared_norm() / (sigma * sigma);
            auto const r6 = r2 * r2 * r2;

            auto const u = 1 / (softness + r6);
            auto const w = u - r6 * u;

            return epsilon * w * w;
        }

        inline md::vector evaluate_force(md::vector r) const
        {
            auto const s_inv2 = 1 / (sigma * sigma);

            auto const r2 = r.squared_norm() * s_inv2;
            auto const r4 = r2 * r2;
            auto const r6 = r4 * r2;

            auto const u = 1 / (softness + r6);
            auto const v = softness * u + u;

            return 12 * epsilon * s_inv2 * r4 * v * (v * u - u) * r;
        }
    };


    //
    // u(r) = e (1 - (k r)^2)^4
    //
    struct softcore_potential
    {
        md::scalar overlap_energy = 1;
        md::scalar cutoff_distance = 1;

        inline md::scalar evaluate_energy(md::vector r) const
        {
            auto const k2 = 1 / (cutoff_distance * cutoff_distance);
            auto const r2 = k2 * r.squared_norm();
            auto const u1 = 1 - r2;

            if (u1 < 0) {
                return 0;
            }

            auto const u2 = u1 * u1;
            auto const u4 = u2 * u2;

            return overlap_energy * u4;
        }

        inline md::vector evaluate_force(md::vector r) const
        {
            auto const k2 = 1 / (cutoff_distance * cutoff_distance);
            auto const r2 = k2 * r.squared_norm();
            auto const u1 = 1 - r2;

            if (u1 < 0) {
                return md::vector {};
            }

            auto const u3 = u1 * u1 * u1;
            auto const coef = 8 * k2 * overlap_energy;

            return coef * u3 * r;
        }
    };
}


//------------------------------------------------------------------------------
// brownian dynamics
//------------------------------------------------------------------------------

#include <cstdint>
#include <random>
#include <vector>


namespace md
{
    struct brownian_dynamics_config
    {
        md::scalar temperature = 1;
        md::scalar timestep = 1;
        md::scalar spacestep = 0;
        std::int64_t steps = 1;
        std::uint64_t seed = 0;
    };

    namespace detail
    {
        inline md::scalar estimate_single_brownian_timestep(
            md::scalar temperature,
            md::scalar mobility,
            md::scalar force,
            md::scalar displacement,
            md::scalar max_timestep
        )
        {
            if (temperature == 0 || mobility == 0 || force == 0) {
                return max_timestep;
            }

            // E[sqrt(e + s R)] ~ sqrt(1 + 2.55 s^2) for a unit vector e, a
            // normally distributed random vector R and a scalar s.
            md::scalar const mean_randomwalk = 2.55;
            md::scalar const epsilon = 1e-6;

            // Solve a*x^2 + b*x - c = 0 for x > 0.
            md::scalar const a = mobility * mobility * force * force;
            md::scalar const b = 2 * mean_randomwalk * mobility * temperature;
            md::scalar const c = displacement * displacement;

            md::scalar const ac_threshold = epsilon * b * b;
            md::scalar timestep;

            if (a * c < ac_threshold) {
                timestep = c / b;
            } else {
                timestep = (-b + std::sqrt(b * b + 4 * a * c)) / (2 * a);
            }

            return std::min(timestep, max_timestep);
        }

        inline md::scalar estimate_brownian_timestep(
            md::system const& system,
            md::brownian_dynamics_config const& config,
            md::array_view<md::vector const> forces
        )
        {
            auto const mobilities = system.mobility_array();
            auto timestep = config.timestep;

            for (md::index i = 0; i < system.particle_count(); i++) {
                timestep = std::min(
                    timestep,
                    detail::estimate_single_brownian_timestep(
                        config.temperature,
                        mobilities[i],
                        forces[i].norm(),
                        config.spacestep,
                        config.timestep
                    )
                );
            }

            return timestep;
        }
    }

    inline void simulate_brownian_dynamics(md::system& system, md::brownian_dynamics_config config)
    {
        std::seed_seq seed {
            unsigned(config.seed >> 32),
            unsigned(config.seed)
        };
        std::mt19937_64 random(seed);
        std::normal_distribution<md::scalar> normal;

        std::vector<md::vector> forces(system.particle_count());
        std::vector<md::vector> weiners(system.particle_count());

        auto mobilities = system.mobility_array();
        auto positions = system.position_array();

        for (std::int64_t step = 0; step < config.steps; step++) {
            // BAOAB limit scheme:
            // q(n+1) = q(n) + mu dt F(n) + sqrt(2 mu dt kT) (R(n) + R(n-1)) / 2

            system.compute_force(forces);

            auto timestep = config.timestep;

            if (config.spacestep > 0) {
                timestep = std::min(
                    timestep,
                    detail::estimate_brownian_timestep(system, config, forces)
                );
            }

            for (md::index i = 0; i < system.particle_count(); i++) {
                auto const drift = mobilities[i] * timestep;
                auto const sigma = std::sqrt(2 * drift * config.temperature);

                auto const weiner = md::vector {
                    sigma * normal(random),
                    sigma * normal(random),
                    sigma * normal(random)
                };

                positions[i] += drift * forces[i];
                positions[i] += 0.5 * (weiner + weiners[i]);
                weiners[i] = weiner;
            }
        }
    }
}


#endif
