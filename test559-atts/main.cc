#include <cstddef>
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>


namespace sim
{
    // attribute_traits maps attribute type T to its value type.
    template<typename T>
    struct attribute_traits
    {
        using value_type = T;
    };

    // attribute_array_base is a base class of attribute_array for type erasure.
    class attribute_array_base
    {
    public:
        virtual ~attribute_array_base() = default;

        // size returns the number of values in this array.
        virtual std::size_t size() const = 0;

        // resize changes the number of values in this array to n.
        virtual void resize(std::size_t n) = 0;
    };

    // attribute_array is an array of T (or its value_type).
    template<typename T>
    class attribute_array : public attribute_array_base
    {
    public:
        using value_type = typename attribute_traits<T>::value_type;

        // size implements attribute_array_base.
        std::size_t size() const override
        {
            return values_.size();
        }

        // resize implements attribute_array_base.
        void resize(std::size_t n) override
        {
            values_.resize(n);
        }

        // XXX: This should return array_view.
        value_type* view() noexcept
        {
            return values_.data();
        }

        // XXX: This should return array_view.
        value_type const* view() const noexcept
        {
            return values_.data();
        }

    private:
        std::vector<value_type> values_;
    };

    class system;

    // particle_proxy delegates access to attributes of a single particle. This is a proxy object
    // and thus should not be stored in a class or returned from a function.
    class particle_proxy
    {
    public:
        // Constructor takes particle's system and index.
        particle_proxy(system& sys, std::size_t index)
            : system_{sys}, index_{index}
        {
        }

        // set assigns value to particle's attribute T.
        template<typename T>
        void set(typename attribute_traits<T>::value_type value) const;

    private:
        system& system_;
        std::size_t index_;
    };

    class system
    {
    public:
        std::size_t particle_count() const noexcept
        {
            return particle_count_;
        }

        particle_proxy add_particle()
        {
            auto const new_count = ++particle_count_;

            for (auto& attr : attributes_) {
                attr.second->resize(new_count);
            }

            return particle_proxy{*this, new_count - 1};
        }

        // XXX: This should return array_view.
        template<typename T>
        typename attribute_traits<T>::value_type* view_attribute()
        {
            return dynamic_cast<attribute_array<T>&>(*attributes_.at(typeid(T))).view();
        }

        // XXX: This should return array_view.
        template<typename T>
        typename attribute_traits<T>::value_type* ensure_attribute()
        {
            std::type_index const key = typeid(T);

            if (attributes_.find(key) == attributes_.end()) {
                auto attr = std::make_unique<attribute_array<T>>();
                attr->resize(particle_count_);
                attributes_.emplace(key, std::move(attr));
            }
            return view_attribute<T>();
        }

    private:
        std::size_t particle_count_ = 0;
        std::unordered_map<std::type_index, std::unique_ptr<attribute_array_base>> attributes_;
    };

    template<typename T>
    void particle_proxy::set(typename attribute_traits<T>::value_type value) const
    {
        system_.ensure_attribute<T>()[index_] = value;
    }
}

int main()
{
    sim::system system;

    system.add_particle().set<double>(1.234);
    system.add_particle().set<int>(999);

    std::cout << "particle count: " << system.particle_count() << '\n';

    auto a = system.view_attribute<double>();
    std::cout << "double: " << a[0] << ' ' << a[1] << '\n';

    auto b = system.view_attribute<int>();
    std::cout << "int: " << b[0] << ' ' << b[1] << '\n';
}
