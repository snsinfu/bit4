#include <cmath>
#include <cstdio>
#include <random>


int main()
{
    double const expected_timestep = 0.001;
    double const max_timestep = 0.01;

    double const spring = 0.01;
    double const epsilon = 0.5;
    double const wavenumber = 0.8;
    double const mobility = 1.0;
    double const temperature = 1.0;

    long const sample_thinning = 10;
    long const sample_count = 10000000;

    std::mt19937_64 engine;
    std::normal_distribution<double> normal_distr;
    std::exponential_distribution<double> timestep_distr{1 / expected_timestep};

    long sample = 0;
    double time = 0;
    double position = 0;
    double prev_weiner = 0;

    for (long step = 0; ; step++) {
        // Variable timestep
        double timestep;
        do {
            timestep = timestep_distr(engine);
        } while (timestep > max_timestep);

        // Force field
        //   u(x) = K/2 x^2 + e sin(kx)
        //   F(x) = -Kx - ek cos(kx)
        double const harmonic_force = -spring * position;
        double const modulation_force = -epsilon * wavenumber * std::cos(wavenumber * position);
        double const force = harmonic_force + modulation_force;

        // Thermal fluctuation
        double const sigma = std::sqrt(2 * mobility * timestep * temperature);
        double const weiner = sigma * normal_distr(engine);

        // BAOAB-limit step
        position += mobility * timestep * force;
        position += (weiner + prev_weiner) / 2;
        time += timestep;
        prev_weiner = weiner;

        // Sampling
        if ((step + 1) % sample_thinning == 0) {
            std::printf("%.6g\t%.4g\n", time, position);

            sample++;
            if (sample == sample_count) {
                break;
            }
        }
    }
}
