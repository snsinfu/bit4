// C++17 class template argument deduction and C++2a designated initializers.

template<int N = 4>
struct softcore_potential
{
    double overlap_energy = 1;
    double cutoff_distance = 1;
};

int main()
{
    // This works
    (void) softcore_potential{};

    // This works
    (void) softcore_potential<4>{
        .overlap_energy  = 50,
        .cutoff_distance = 0.1
    };

    // This doesn't!
    (void) softcore_potential{
        .overlap_energy  = 50,
        .cutoff_distance = 0.1
    };
}
