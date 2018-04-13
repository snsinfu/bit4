#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>


template<typename T>
struct widget
{
    widget(T&& object1, T&& object2)
        : object1_{std::forward<T>(object1)}
        , object2_{std::forward<T>(object2)}
    {
    }

    void use()
    {
        std::cout << static_cast<void*>(&object1_.at(0))
                  << '\t'
                  << static_cast<void*>(&object2_.at(0))
                  << '\n';
    }

  private:

    // These variables are references if T&& is an lvalue reference, or values
    // if T&& is an rvalue reference.

    T object1_;
    T object2_;
};

template<typename T>
widget<T> make_widget(T&& obj)
{
    // Pass references to obj if T&& is an lvalue reference, or pass copies of
    // obj if T&& is an rvalue reference.
    return widget<T>{T{obj}, T{obj}};
}

int main()
{
    std::vector<int> resource(100);

    auto w1 = make_widget(resource);
    auto w2 = make_widget(std::vector<int>{1, 2, 3, 4});

    w1.use();
    w2.use();
}
