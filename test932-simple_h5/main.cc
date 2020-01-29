#include <algorithm>
#include <cstdio>
#include <exception>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "h5_store.hpp"


int main()
{
    h5_store store{"_test.h5"};

    std::vector<float> numbers(100);
    store.write_array("dataset", numbers.data(), numbers.size());
}
