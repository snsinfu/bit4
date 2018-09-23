#ifndef INCLUDED_SIM_HPP
#define INCLUDED_SIM_HPP

#include <cstddef>

#include "array_view.hpp"
#include "geo.hpp"


namespace sim
{
    using index = std::size_t;
    using scalar = double;
    using vector = geo::vector<sim::scalar, 3>;
    using point = geo::point<sim::scalar, 3>;
    using cxx::array_view;
}

//------------------------------------------------------------------------------

namespace sim
{
    class system;

    // forcefield is a base class of mechanical force fields.
    class forcefield
    {
    public:
        virtual ~forcefield() = default;

        // compute_energy computes potential energy of the system.
        virtual sim::scalar compute_energy(sim::system const& system) = 0;

        // compute_force computes force acting on the particles in the system
        // and adds calculated force vectors into force_array.
        virtual void compute_force(
            sim::system const& system,
            sim::array_view<sim::vector> force_array
        ) = 0;
    };
}


//------------------------------------------------------------------------------

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>


namespace sim
{
    template<typename T>
    struct basic_attribute_traits
    {
        using value_type = T;
        static inline constexpr value_type default_value() { return value_type(); }
    };

    // attribute_traits defines traits of a particle attribute identified by
    // type T.
    template<typename T>
    struct attribute_traits : basic_attribute_traits<T>
    {
    };

    namespace detail
    {
        // attribute_base is the base class of type-erased arrays.
        class attribute_base
        {
        public:
            virtual ~attribute_base() = default;

            // resize resizes the internal storage to n-items length.
            virtual void resize(sim::index n) = 0;
        };

        // attribute stores an array of values identified by type T.
        template<typename T>
        class attribute : public attribute_base
        {
        public:
            using value_type = typename sim::attribute_traits<T>::value_type;

            void resize(sim::index n) override
            {
                values_.resize(n, sim::attribute_traits<T>::default_value());
            }

            inline sim::array_view<value_type> view() noexcept
            {
                return values_;
            }

        private:
            std::vector<value_type> values_;
        };

        // attribute_table is a set of columnar arrays with the same lengths.
        // Each column is keyed with a static type.
        class attribute_table
        {
        public:
            // size returns the common length of held arrays.
            inline sim::index size() const noexcept
            {
                return size_;
            }

            // resize changes the size of all held arrays.
            void resize(sim::index n)
            {
                for (auto& key_column : attrs_) {
                    key_column.second->resize(n);
                }
                size_ = n;
            }

            // make_attribute creates a column keyed with type T if it does not
            // exist yet.
            template<typename T>
            sim::array_view<typename sim::attribute_traits<T>::value_type> make_attribute()
            {
                std::type_index const key = typeid(T);

                if (attrs_.find(key) == attrs_.end()) {
                    auto attr = std::make_unique<detail::attribute<T>>();
                    attr->resize(size_);
                    attrs_.emplace(key, std::move(attr));
                }

                return view_attribute<T>();
            }

            // view_attribute
            template<typename T>
            inline sim::array_view<typename sim::attribute_traits<T>::value_type>
            view_attribute()
            {
                return static_cast<detail::attribute<T>&>(*attrs_.at(typeid(T))).view();
            }

            // view_attribute
            template<typename T>
            inline sim::array_view<typename sim::attribute_traits<T>::value_type const>
            view_attribute() const
            {
                return static_cast<detail::attribute<T>&>(*attrs_.at(typeid(T))).view();
            }

        private:
            std::unordered_map<std::type_index, std::unique_ptr<detail::attribute_base>> attrs_;
            sim::index size_ = 0;
        };
    }
}


//------------------------------------------------------------------------------

#include <memory>
#include <vector>


namespace sim
{
    namespace detail
    {
        // sum_forcefield composes zero or more child forcefield instances into
        // a single forcefield. This class is used by sim::system.
        class sum_forcefield : public virtual sim::forcefield
        {
        public:
            // add_forcefield adds a forcefield as a child.
            void add_forcefield(std::shared_ptr<sim::forcefield> forcefield)
            {
                forcefields_.push_back(forcefield);
            }

            // compute_energy computes the sum of potential energy values
            // computed by child forcefield instances.
            sim::scalar compute_energy(sim::system const& system) override
            {
                sim::scalar sum = 0;
                for (auto& forcefield : forcefields_) {
                    sum += forcefield->compute_energy(system);
                }
                return sum;
            }

            // compute_force computes the sum of force vectors computed by child
            // forcefield instances.
            void compute_force(
                sim::system const& system,
                sim::array_view<sim::vector> force_array
            ) override
            {
                for (auto& forcefield : forcefields_) {
                    forcefield->compute_force(system, force_array);
                }
            }

        private:
            std::vector<std::shared_ptr<sim::forcefield>> forcefields_;
        };
    }
}


//------------------------------------------------------------------------------

#include <memory>


namespace sim
{
    // mass_attribute is an attribute tag for particle mass.
    struct mass_attribute {};

    template<>
    struct attribute_traits<sim::mass_attribute> : basic_attribute_traits<sim::scalar>
    {
        static inline constexpr value_type default_value() { return 1; }
    };

    // position_attribute is an attribute tag for particle position.
    struct position_attribute {};

    template<>
    struct attribute_traits<sim::position_attribute> : basic_attribute_traits<sim::point> {};

    // velocity_attribute is an attribute tag for particle velocity.
    struct velocity_attribute {};

    template<>
    struct attribute_traits<sim::velocity_attribute> : basic_attribute_traits<sim::vector> {};

    // system holds particles and associated forcefield.
    class system
    {
    public:
        system()
        {
            attr_table_.make_attribute<sim::mass_attribute>();
            attr_table_.make_attribute<sim::position_attribute>();
            attr_table_.make_attribute<sim::velocity_attribute>();
        }

        // particle_count returns the number of particles in the system.
        inline sim::index particle_count() const noexcept
        {
            return attr_table_.size();
        }

        // add_particle adds a particle into the system.
        void add_particle()
        {
            attr_table_.resize(attr_table_.size() + 1);
        }

        // add_forcefield adds a forcefield into the system.
        void add_forcefield(std::shared_ptr<sim::forcefield> forcefield)
        {
            forcefield_.add_forcefield(forcefield);
        }

        // make_attribute ensures the existence of particle attribute T. Returns
        // a mutable view of attribtue values.
        template<typename T>
        sim::array_view<typename sim::attribute_traits<T>::value_type> make_attribute()
        {
            return attr_table_.make_attribute<T>();
        }

        // view_attribute returns a mutable view of attribute values.
        template<typename T>
        inline sim::array_view<typename sim::attribute_traits<T>::value_type> view_attribute()
        {
            return attr_table_.view_attribute<T>();
        }

        // view_attribute returns a read-only view of attribute values.
        template<typename T>
        inline sim::array_view<typename sim::attribute_traits<T>::value_type const>
        view_attribute() const
        {
            return attr_table_.view_attribute<T>();
        }

        // view_mass_array returns a mutable view of particle mass.
        inline sim::array_view<sim::scalar> view_mass_array() noexcept
        {
            return attr_table_.view_attribute<sim::mass_attribute>();
        }

        // view_mass_array returns a read-only view of particle mass.
        inline sim::array_view<sim::scalar const> view_mass_array() const noexcept
        {
            return attr_table_.view_attribute<sim::mass_attribute>();
        }

        // view_position_array returns a mutable view of particle position.
        inline sim::array_view<sim::point> view_position_array() noexcept
        {
            return attr_table_.view_attribute<sim::position_attribute>();
        }

        // view_position_array returns a read-only view of particle position.
        inline sim::array_view<sim::point const> view_position_array() const noexcept
        {
            return attr_table_.view_attribute<sim::position_attribute>();
        }

        // view_velocity_array returns a mutable view of particle velocity.
        inline sim::array_view<sim::vector> view_velocity_array() noexcept
        {
            return attr_table_.view_attribute<sim::velocity_attribute>();
        }

        // view_velocity_array returns a read-only view of particle velocity.
        inline sim::array_view<sim::vector const> view_velocity_array() const noexcept
        {
            return attr_table_.view_attribute<sim::velocity_attribute>();
        }

        // compute_potential_energy computes the total potential energy of the
        // particles in the system.
        sim::scalar compute_potential_energy()
        {
            return forcefield_.compute_energy(*this);
        }

        // compute_force computes the force acting on all particles.
        void compute_force(sim::array_view<sim::vector> force_array)
        {
            if (force_array.size() != particle_count()) {
                throw std::logic_error(
                    "sim::system::compute_force - "
                    "size of force_array is not equal to particle_count"
                );
            }

            for (auto& force : force_array) {
                force = {}; // Zero clear
            }

            forcefield_.compute_force(*this, force_array);
        }

    private:
        detail::attribute_table attr_table_;
        detail::sum_forcefield forcefield_;
    };
}


//------------------------------------------------------------------------------

namespace sim
{
    // composite_forcefield is a static composition of forcefield classes.
    template<typename... Components>
    class composite_forcefield : public Components..., virtual sim::forcefield
    {
    public:
        composite_forcefield() = default;

        template<typename... Inits>
        composite_forcefield(Inits const&... inits)
            : Inits(inits)...
        {
        }

        // compute_energy computes the sum of energy values computed by the
        // components.
        sim::scalar compute_energy(sim::system const& system) override
        {
            sim::scalar sum = 0;
            for (auto energy : {Components::compute_energy(system)...}) {
                sum += energy;
            }
            return sum;
        }

        // compute_force computes the sum of force vectors computed by the
        // components.
        void compute_force(
            sim::system const& system,
            sim::array_view<sim::vector> force_array
        ) override
        {
            int dummy[] = {
                0, (Components::compute_force(system, force_array), 0)...
            };
            (void) dummy;
        }
    };

    template<typename Derived>
    class pairwise_forcefield : public virtual sim::forcefield
    {
    public:
        sim::scalar compute_energy(sim::system const& system) override
        {
            sim::scalar sum = 0;

            auto const position_array = system.view_position_array();

            for (sim::index j = 0; j < system.particle_count(); j++) {
                auto const position_j = position_array[j];
                for (sim::index i = 0; i < j; i++) {
                    auto const potential = derived().pairwise_potential(system, i, j);
                    auto const r = position_array[i] - position_j;
                    sum += potential.evaluate_energy(r);
                }
            }

            return sum;
        }

        void compute_force(
            sim::system const& system,
            sim::array_view<sim::vector> force_array
        ) override
        {
            auto const position_array = system.view_position_array();

            for (sim::index j = 0; j < system.particle_count(); j++) {
                auto const position_j = position_array[j];
                sim::vector force_j;
                for (sim::index i = 0; i < j; i++) {
                    auto const potential = derived().pairwise_potential(system, i, j);
                    auto const r = position_array[i] - position_j;
                    auto const f = potential.evaluate_force(r);
                    force_array[i] += f;
                    force_j -= f;
                }
                force_array[j] += force_j;
            }
        }

    private:
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }
    };
}


//------------------------------------------------------------------------------

namespace sim
{
    struct harmonic_potential
    {
        sim::scalar spring_constant = 1;

        inline sim::scalar evaluate_energy(sim::vector r) const
        {
            return 0.5 * spring_constant * r.squared_norm();
        }

        inline sim::vector evaluate_force(sim::vector r) const
        {
            return -spring_constant * r;
        }
    };

    struct softcore_lennard_jones_potential
    {
        sim::scalar epsilon = 1;
        sim::scalar sigma = 1;
        sim::scalar softness = 1;

        inline sim::scalar evaluate_energy(sim::vector r) const
        {
            auto const r2 = r.squared_norm() / (sigma * sigma);
            auto const r6 = r2 * r2 * r2;

            auto const u = 1 / (softness + r6);
            auto const w = u - r6 * u;

            return epsilon * w * w;
        }

        inline sim::vector evaluate_force(sim::vector r) const
        {
            auto const s2 = 1 / (sigma * sigma);
            auto const r2 = r.squared_norm() * s2;
            auto const r4 = r2 * r2;
            auto const r6 = r4 * r2;

            auto const u = 1 / (softness + r6);
            auto const v = softness * u + u;

            return 12 * epsilon * s2 * r4 * v * (v * u - u) * r;
        }
    };
}


#endif
