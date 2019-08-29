#include <algorithm>
#include <iostream>
#include <set>


int main()
{
    std::set<int> set = {1, 2, 3};
    std::multiset<int> multiset1 = {1, 2, 3};
    std::multiset<int> multiset2 = {1, 1, 2, 3};

    // error: invalid operands to binary expression
    //std::cout << (set == multiset) << '\n';

    // OK, prints 1
    std::cout << std::equal(set.begin(), set.end(), multiset1.begin(), multiset1.end()) << '\n';

    // OK, prints 0
    std::cout << std::equal(set.begin(), set.end(), multiset2.begin(), multiset2.end()) << '\n';
}
