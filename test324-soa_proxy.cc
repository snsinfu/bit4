// Design of proxy class for accessing SoA-stored elements.

// Proxy objects do not own accessed values. They are basically pointers and
// thus should not be copied around.
//
// We can prevent unwanted copies by deleting copy and move constructors, but
// such an object is mostly unusable before C++17 guaranteed copy elision. And
// even with C++17, it is not ideal in terms of API ergonomics; classes should
// mimic fundamental C++ types. Non-copy/moveable objects look irregular!
//
// Now, recall that proxy objects are basically pointers. So, how about making
// them look like pointers?
//
//      proxy = collection.view(i);
//      proxy->foo();
//
// Note the arrow operator. With this syntax, a good C++ programmer would
// naturally hesitate to copy around the proxy object.

//------------------------------------------------------------------------------

// Wraps a value and provides access to the stored value via pointer syntax.
template<typename T>
class pointer_semantic
{
    using value_type = T;

  public:
    // Needs C++17 to work.
    //pointer_semantic(pointer_semantic const&) = delete;
    //pointer_semantic& operator=(pointer_semantic const&) = delete;

    explicit pointer_semantic(value_type const& value)
        : value_{value}
    {
    }

    value_type const* operator->() const
    {
        return &value_;
    }

  private:
    value_type value_;
};

//------------------------------------------------------------------------------

#include <cassert>
#include <cstddef>
#include <vector>

struct point
{
    double x;
    double y;
};

inline bool operator==(point const& lhs, point const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

struct color
{
    double r;
    double g;
    double b;
};

inline bool operator==(color const& lhs, color const& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

// particle_cloud is an array of particles, and each particle has point and
// color attribute:
//
//      particle_cloud = [particle, particle, ..., particle]
//      particle = (point, color)
//
// This class stores these attributes in a struct-of-array (or columnar) layout
// and provides array-of-struct access to each particle via a proxy object.
class particle_cloud
{
  public:
    class particle_proxy;

    std::size_t size() const
    {
        return points_.size();
    }

    // Defined out of line, after the definition of particle_proxy.
    pointer_semantic<particle_proxy> view(std::size_t index) const;

    void add(point const& pt, color const& col)
    {
        points_.push_back(pt);
        colors_.push_back(col);
    }

  private:
    std::vector<point> points_;
    std::vector<color> colors_;
};

class particle_cloud::particle_proxy
{
    friend particle_cloud;

    particle_proxy(particle_cloud const& cloud, std::size_t index)
        : cloud_{cloud}
        , index_{index}
    {
    }

  public:
    point const& point() const
    {
        return cloud_.points_[index_];
    }

    color const& color() const
    {
        return cloud_.colors_[index_];
    }

  private:
    particle_cloud const& cloud_;
    std::size_t index_;

};

inline
pointer_semantic<particle_cloud::particle_proxy> particle_cloud::view(std::size_t index) const
{
    return pointer_semantic<particle_proxy>{particle_proxy{*this, index}};
}

int main()
{
    particle_cloud pc;

    pc.add({0.0, 1.0}, {1.0, 0.0, 0.0});
    pc.add({0.5, 0.8}, {0.0, 1.0, 0.0});
    pc.add({0.9, 2.1}, {0.0, 0.0, 1.0});

    assert(pc.view(0)->point() == (point{0.0, 1.0}));
    assert(pc.view(1)->color() == (color{0.0, 1.0, 0.0}));
    assert(pc.view(2)->color() == (color{0.0, 0.0, 1.0}));

    auto proxy = pc.view(0);
    proxy->point();
    proxy->color();
}
