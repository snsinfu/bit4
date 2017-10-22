class coords_storage
{
  public:
    coords_storage() = default;

    explicit coords_storage(int x, int y, int z)
        : coords_{x, y, z}
    {
    }

  protected:
    int coords_[3] {};
};

template<bool AllowImplicit>
class coords_mixin;

template<>
class coords_mixin<true> : public coords_storage
{
  public:
    coords_mixin() = default;

    coords_mixin(int x, int y, int z)
        : coords_storage{x, y, z}
    {
    }
};

template<>
class coords_mixin<false> : public coords_storage
{
  public:
    coords_mixin() = default;

    explicit coords_mixin(int x, int y, int z)
        : coords_storage{x, y, z}
    {
    }
};

class implicit_vector : private coords_mixin<true>
{
    using coords_storage::coords_;

  public:
    using coords_mixin<true>::coords_mixin;

    int get_x() const
    {
        return coords_[0];
    }
};

class explicit_vector : private coords_mixin<false>
{
    using coords_storage::coords_;

  public:
    using coords_mixin<false>::coords_mixin;

    int get_x() const
    {
        return coords_[0];
    }
};

int main()
{
    implicit_vector v = {1, 2, 3};
    explicit_vector w{1, 2, 3};
    v.get_x();
    w.get_x();
}
