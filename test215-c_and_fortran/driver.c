#include <stdio.h>
#include <stdlib.h>

struct system
{
    int    particle_count;
    double spring_constant;
};

static void *xcalloc(size_t count, size_t size);

extern void init_positions_(struct system *sys, double *pos);
extern void compute_force_and_energy_(struct system *sys, double *pos, double *forces, double *energy);

int main()
{
    struct system sys = {
        .particle_count   = 1000,
        .spring_constant  = 1.0,
    };
    double *positions = xcalloc(1000, 3 * sizeof(double));
    double *forces = xcalloc(1000, 3 * sizeof(double));
    double energy;

    init_positions_(&sys, positions);
    compute_force_and_energy_(&sys, positions, forces, &energy);

    printf("%g\n", energy);

    free(forces);
    free(positions);
}

void *xcalloc(size_t count, size_t size)
{
    void *p = calloc(count, size);
    if (p == NULL) {
        perror("calloc failed");
        exit(EXIT_FAILURE);
    }
    return p;
}
