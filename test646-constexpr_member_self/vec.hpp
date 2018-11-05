namespace vec
{
    struct vector
    {
        double x = 0;
        double y = 0;
        double z = 0;

        static const vector zero;
    };

    // This works perfectly but requires C++17.
    // inline constexpr const vector vector::zero {};

    // duplicate symbol __ZN3vec6vector4zeroE in:
    //     a.o
    //     b.o
    constexpr const vector vector::zero {};
}
