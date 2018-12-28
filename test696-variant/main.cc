#include <iostream>
#include <string>
#include <variant>


int main()
{
    std::variant<int, double, std::string> var;

    var = 1.23;

    auto visitor = [](auto const& val) {
        std::cout << val << '\n';
    };
    std::visit(visitor, var);
}
