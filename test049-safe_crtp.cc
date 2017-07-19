#include <iostream>
#include <cassert>

template<typename Derived>
struct Base
{
    void foo()
    {
        derived().do_foo();
    }

  protected:
    virtual
    ~Base() = default;

  private:
    inline
    Derived& derived() noexcept
    {
        assert(dynamic_cast<Derived*>(this));
        return static_cast<Derived&>(*this);
    }
};

struct DerivedA : Base<DerivedA>
{
    void do_foo()
    {
        std::cout << "A " << value << '\n';
    }

    int value = 10;
};

struct DerivedB : Base<DerivedA> // oops!
{
    void do_foo()
    {
        std::cout << "B\n";
    }
};

int main()
{
    DerivedB b;
    b.foo(); // undefined behavior
}
