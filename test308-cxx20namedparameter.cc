namespace
{
    class mass_t
    {
      public:
        explicit mass_t(double value)
            : value_{value}
        {
        }

      private:
        double value_;
    };

    struct particle
    {
        mass_t mass;
    };
}

int main()
{
    // Works with clang++ and g++
    particle equalinit{
        .mass = mass_t{1.23}
    };

    // This does not work yet
    particle braceinit{
        .mass{1.23}
    };
}
