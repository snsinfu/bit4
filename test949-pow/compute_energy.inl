static double evaluate_lennard_jones(double dx, double epsilon, double sigma);


double compute_energy(double const *positions, int size)
{
    double const epsilon = 1;
    double const sigma = 0.1;
    double energy = 0;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            double dx = positions[i] - positions[j];
            energy += evaluate_lennard_jones(dx, epsilon, sigma);
        }
    }

    return energy;
}
