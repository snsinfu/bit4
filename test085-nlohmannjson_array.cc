#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#include <json.hpp>

int main()
{
    nlohmann::json const config = nlohmann::json::parse(R"({
        "direction": [1.0, 2.0, 3.0]
    })");

    // error: no member named 'insert' in 'std::__1::array<double, 3>'
    // std::array<double, 3> dir = config["direction"];
    std::vector<double> dir = config["direction"];

    assert(dir.size() == 3);
    assert(dir[0] == 1.0);
    assert(dir[1] == 2.0);
    assert(dir[2] == 3.0);

    std::cout << dir[0] << ' ' << dir[1] << ' ' << dir[2] << '\n';
}
