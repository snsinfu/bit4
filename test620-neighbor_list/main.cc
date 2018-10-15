#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>


struct point
{
    double x = 0;
    double y = 0;
    double z = 0;
};


unsigned hash(unsigned x, unsigned y, unsigned z, unsigned nbins)
{
    constexpr unsigned x_stride = 73856093;
    constexpr unsigned y_stride = 19349669;
    constexpr unsigned z_stride = 83492791;

    return (x * x_stride + y * y_stride + z * z_stride) % nbins;
}


unsigned locate(point pt, double dcut, unsigned nbins)
{
    auto const freq = 1 / dcut;

    auto const x = unsigned(int(std::nearbyint(freq * pt.x)));
    auto const y = unsigned(int(std::nearbyint(freq * pt.y)));
    auto const z = unsigned(int(std::nearbyint(freq * pt.z)));

    return hash(x, y, z, nbins);
}


int main()
{
    unsigned const nbins = 50;
    double const dcut = 0.2;

    std::vector<unsigned> deltas;
    for (unsigned dx : {nbins - 1, nbins, nbins + 1}) {
        for (unsigned dy : {nbins - 1, nbins, nbins + 1}) {
            for (unsigned dz : {nbins - 1, nbins, nbins + 1}) {
                deltas.push_back(hash(dx, dy, dz, nbins));
            }
        }
    }
    std::sort(deltas.begin(), deltas.end());
    deltas.erase(std::unique(deltas.begin(), deltas.end()), deltas.end());

    std::cout << "deltas:";
    for (auto delta : deltas) {
        std::cout << ' ' << delta;
    }
    std::cout << '\n';

    point const p = {0.431838, 2.15173, -1.02672};
    point const q = {0.228886, 2.3595, -0.445631};
    point const r = {0.690411, 2.8247, -1.12345};

    std::cout << "p: " << locate(p, dcut, nbins) << '\n';
    std::cout << "q: " << locate(q, dcut, nbins) << '\n';
    std::cout << "r: " << locate(r, dcut, nbins) << '\n';
}
