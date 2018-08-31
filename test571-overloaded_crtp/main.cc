#include <iostream>

namespace
{
    struct state
    {
    };

    template<typename Derived>
    class pairwise_interaction
    {
    public:
        double evaluate(state& s)
        {
            double sum = 0;

            for (int j = 0; j < 10; j++) {
                for (int i = 0; i < j; i++) {
                    sum += derived().pairwise_energy(s, i, j);
                }
            }

            return sum;
        }

        inline double pairwise_energy(state&, int i, int j)
        {
            return derived().pairwise_energy(i, j);
        }

        inline double pairwise_energy(int, int)
        {
            return derived().pairwise_energy();
        }

    private:
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }
    };

    class my_interaction_1 : public pairwise_interaction<my_interaction_1>
    {
    public:
        using pairwise_interaction<my_interaction_1>::pairwise_energy;

        inline double pairwise_energy()
        {
            return 1.2;
        }
    };

    class my_interaction_2 : public pairwise_interaction<my_interaction_2>
    {
    public:
        using pairwise_interaction<my_interaction_2>::pairwise_energy;

        inline double pairwise_energy(int i, int j)
        {
            return 1.2 + (i * j);
        }
    };

    class my_interaction_3 : public pairwise_interaction<my_interaction_3>
    {
    public:
        using pairwise_interaction<my_interaction_3>::pairwise_energy;

        inline double pairwise_energy(state&, int i, int j)
        {
            return 1.2 + (i * j);
        }
    };
}

int main()
{
    state s;
    my_interaction_2 m;
    std::cout << m.evaluate(s) << '\n';
}
