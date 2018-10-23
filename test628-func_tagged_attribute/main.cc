#include <vector>


namespace md
{
    template<typename T, typename Tag = T>
    using attribute_key = T(*)(Tag*);

    template<typename T, typename Tag>
    T default_value(md::attribute_key<T, Tag> key)
    {
        return key ? key(nullptr) : T{};
    }

    struct system
    {
        template<typename T, typename Tag>
        void require(md::attribute_key<T, Tag> key)
        {
            md::default_value(key);
        }

        template<typename T, typename Tag>
        std::vector<T> view(md::attribute_key<T, Tag> key)
        {
            return std::vector<T>(10, md::default_value(key));
        }
    };

    inline double mass_attribute(struct mass_attribute_tag*)
    {
        return 1;
    }
}


struct particle_params
{
    int kind = 0;
    double radius = 1;
};

md::attribute_key<particle_params> const particle_params_attribute = {};


int main()
{
    md::system system;

    system.require(md::mass_attribute);
    system.require(particle_params_attribute);

    for (particle_params& params : system.view(particle_params_attribute)) {
        params.kind = 1;
        params.radius *= 2;
    }
}
