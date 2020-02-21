#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static double timestamp();
static double run_benchmark(int size, int steps);
static void   randomize_positions(double *positions, int size);


// Implemented differently in mul.c, pow.c or intpow.c.
double compute_energy(double const *positions, int size);


int main(void)
{
    int const bench_size = 1000;
    int const bench_steps = 1000;

    double start_time = timestamp();
    double result = run_benchmark(bench_size, bench_steps);
    double end_time = timestamp();

    double elapsed_time = end_time - start_time;
    printf("%g s\t%g\n", elapsed_time, result);
}


static
double timestamp()
{
    struct timespec tp;

    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp) == -1) {
        return NAN;
    }
    return (double) tp.tv_sec + (double) tp.tv_nsec * 1e-9;
}


static
double run_benchmark(int size, int steps)
{
    double *positions = calloc(sizeof(double), (size_t) size);

    double sum = 0;
    for (int step = 0; step < steps; step++) {
        randomize_positions(positions, size);
        sum += compute_energy(positions, size);
    }

    free(positions);

    return sum / (double) steps;
}


static
void randomize_positions(double *positions, int size)
{
    double spacing = 0.1 + 0.1 * (double) rand() / (double) RAND_MAX;
    for (int i = 0; i < size; i++) {
        positions[i] = (double) i * spacing;
    }
}
