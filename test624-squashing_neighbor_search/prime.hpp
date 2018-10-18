#ifndef INCLUDED_PRIME_HPP
#define INCLUDED_PRIME_HPP

#include "types.hpp"


namespace md
{
    inline bool is_prime(md::index num)
    {
        if (num % 2 == 0 || num % 3 == 0) {
            return false;
        }

        for (md::index iter = 6; iter < num / 2; iter++) {
            if (num % (iter - 1) == 0 || num % (iter + 1) == 0) {
                return false;
            }
        }

        return true;
    }

    inline md::index next_prime(md::index num)
    {
        for (md::index iter = (num / 6 + 1) * 6; ; iter += 6) {
            if (md::is_prime(iter - 1)) {
                return iter - 1;
            }
            if (md::is_prime(iter + 1)) {
                return iter + 1;
            }
        }
    }
}

#endif
