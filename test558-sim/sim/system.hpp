// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_SIM_SYSTEM_HPP
#define INCLUDED_SIM_SYSTEM_HPP

#include <cmath>
#include <cstddef>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <geo/geo.hpp>
#include <ext/array_view.hpp>


namespace sim
{
    using index = std::size_t;
    using scalar = double;
    using point = geo::point<scalar, 3>;
    using vector = geo::vector<scalar, 3>;
    using ext::array_view;

    template<typename T>
    struct attribute_traits
    {
        using value_type = T;
    };

    //
    struct velocity_attribute {};

    template<>
    struct attribute_traits<sim::velocity_attribute>
    {
        using value_type = sim::vector;
    };

    //
    struct force_attribute {};

    template<>
    struct attribute_traits<sim::force_attribute>
    {
        using value_type = sim::vector;
    };

    //
    struct mass_attribute {};

    template<>
    struct attribute_traits<sim::mass_attribute>
    {
        using value_type = sim::scalar;
    };

    namespace detail
    {
        class attribute_base
        {
        public:
            virtual ~attribute_base() = default;
            virtual void resize(index n) = 0;
        };

        template<typename T>
        class attribute : public attribute_base
        {
        public:
            using value_type = typename attribute_traits<T>::value_type;

            void resize(index n) override
            {
                values_.resize(n);
            }

            array_view<value_type> view() noexcept
            {
                return ext::make_array_view(values_);
            }

        private:
            std::vector<value_type> values_;
        };
    }

    class system;

    class particle_proxy
    {
    public:
        particle_proxy(sim::system& system, sim::index index)
            : system_{system}, index_{index}
        {
        }

        template<typename T>
        void set(typename attribute_traits<T>::value_type value);

    private:
        sim::system& system_;
        sim::index index_;
    };

    struct basic_attributes
    {
        sim::scalar mass = 1;
        sim::point point;
        sim::vector velocity;
        sim::vector force;
    };

    class system
    {
    public:
        system()
        {
            make_attribute<sim::point>();
            make_attribute<sim::velocity_attribute>();
            make_attribute<sim::force_attribute>();
            make_attribute<sim::mass_attribute>();
        }

        sim::index particle_count() const
        {
            return particle_count_;
        }

        sim::particle_proxy add_particle(sim::basic_attributes const& basic = {})
        {
            auto const new_count = particle_count_ + 1;

            for (auto& attr : attributes_) {
                attr.second->resize(new_count);
            }

            auto const index = particle_count_;
            view_point()[index] = basic.point;
            view_velocity()[index] = basic.velocity;
            view_force()[index] = basic.force;
            view_mass()[index] = basic.mass;

            particle_count_ = new_count;

            return particle_proxy{*this, index};
        }

        template<typename T>
        array_view<typename attribute_traits<T>::value_type> make_attribute()
        {
            std::type_index const key = typeid(T);

            if (attributes_.find(key) == attributes_.end()) {
                auto attr = std::make_unique<detail::attribute<T>>();
                attr->resize(particle_count_);
                attributes_.emplace(key, std::move(attr));
            }

            return view_attribute<T>();
        }

        template<typename T>
        array_view<typename attribute_traits<T>::value_type> view_attribute()
        {
            return dynamic_cast<detail::attribute<T>&>(*attributes_.at(typeid(T))).view();
        }

        template<typename T>
        array_view<typename attribute_traits<T>::value_type const> view_attribute() const
        {
            return dynamic_cast<detail::attribute<T>&>(*attributes_.at(typeid(T))).view();
        }

        // Convenience accessors for the basic attributes.

        // view_point returns a view into the array of particle points.
        sim::array_view<sim::point> view_point() noexcept
        {
            return view_attribute<sim::point>();
        }

        sim::array_view<sim::point const> view_point() const noexcept
        {
            return view_attribute<sim::point>();
        }

        // view_velocity returns a view into the array of particle velocities.
        sim::array_view<sim::vector> view_velocity() noexcept
        {
            return view_attribute<sim::velocity_attribute>();
        }

        sim::array_view<sim::vector const> view_velocity() const noexcept
        {
            return view_attribute<sim::velocity_attribute>();
        }

        // view_force returns a view into the array of forces acting on particles.
        sim::array_view<sim::vector> view_force() noexcept
        {
            return view_attribute<sim::force_attribute>();
        }

        sim::array_view<sim::vector const> view_force() const noexcept
        {
            return view_attribute<sim::force_attribute>();
        }

        // view_mass returns a view into the array of particle masses.
        sim::array_view<sim::scalar> view_mass() noexcept
        {
            return view_attribute<sim::mass_attribute>();
        }

        sim::array_view<sim::scalar const> view_mass() const noexcept
        {
            return view_attribute<sim::mass_attribute>();
        }

    private:
        sim::index particle_count_ = 0;
        std::unordered_map<std::type_index, std::unique_ptr<detail::attribute_base>> attributes_;
    };

    template<typename T>
    void particle_proxy::set(typename attribute_traits<T>::value_type value)
    {
        system_.make_attribute<T>()[index_] = value;
    }
}

#endif
