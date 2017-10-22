namespace sandbox
{
    template<typename...>
    struct type_sequence
    {
    };

    template<typename T, unsigned N, typename... Seq>
    struct repeat_type
    {
        using type = typename repeat_type<T, N - 1, T, Seq...>::type;
    };

    template<typename T, typename... Seq>
    struct repeat_type<T, 0, Seq...>
    {
        using type = type_sequence<Seq...>;
    };

    template<typename T, unsigned N, typename CoordsSeq = typename repeat_type<T, N>::type>
    class coords_mixin;

    template<typename T, unsigned N, typename... Coords>
    class coords_mixin<T, N, type_sequence<Coords...>>
    {
      public:
        coords_mixin() = default;

        coords_mixin(Coords... coords)
            : coords_{coords...}
        {
        }

      protected:
        T coords_[N] {};
    };

    template<typename T, unsigned N>
    class vector : public coords_mixin<T, N>
    {
      public:
        using coords_mixin<T, N>::coords_mixin;
    };
}

int main()
{
    sandbox::vector<double, 3> v = {1, 2, 3};
}
