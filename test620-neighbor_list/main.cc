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


double distance(point p, point q)
{
    auto const dx = p.x - q.x;
    auto const dy = p.y - q.y;
    auto const dz = p.z - q.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}


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

    point const p = {0.326633, -1.03745, -0.0764704};
    point const q = {0.236804, -1.13701, 0.0440195};
    point const r = {0.384521, -1.14729, -0.138036};

    auto const pbin = locate(p, dcut, nbins);
    auto const qbin = locate(q, dcut, nbins);
    auto const rbin = locate(r, dcut, nbins);

    std::cout << '\n';

    std::cout << "bin(p): " << pbin << '\n';
    std::cout << "bin(q): " << qbin << '\n';
    std::cout << "bin(r): " << rbin << '\n';

    std::cout << '\n';

    std::cout << "bin(q) - bin(p): " << (qbin + nbins - pbin) % nbins << '\n';
    std::cout << "bin(r) - bin(p): " << (rbin + nbins - pbin) % nbins << '\n';

    std::cout << '\n';

    std::cout << "d(p,q): " << distance(p, q) << '\n';
    std::cout << "d(p,r): " << distance(p, r) << '\n';
}
