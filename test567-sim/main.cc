//-------------------------------------------------------------------------------------------------

#include <cstddef>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include <ext/array_view.hpp>
#include <geo/geo.hpp>


namespace sim
{
    using index = std::size_t;
    using scalar = double;
    using point = geo::point<sim::scalar, 3>;
    using vector = geo::vector<sim::scalar, 3>;
    using ext::array_view;

    // basic_property_traits
    template<typename T>
    struct basic_property_traits
    {
        using value_type = T;

        static constexpr value_type default_value() noexcept
        {
            return value_type{};
        }
    };

    // property_traits
    template<typename T>
    struct property_traits : basic_property_traits<T>
    {
    };

    // property_value_t
    template<typename T>
    using property_value_t = typename property_traits<T>::value_type;

    namespace detail
    {
        // property_base
        class property_base
        {
        public:
            virtual ~property_base() = default;
            virtual void resize(sim::index n) = 0;
        };

        // property
        template<typename T>
        class property : public property_base
        {
        public:
            using value_type = sim::property_value_t<T>;

            // resize
            void resize(sim::index n) override
            {
                values_.resize(n, sim::property_traits<T>::default_value());
            }

            // view
            sim::array_view<value_type> view() noexcept
            {
                return ext::make_array_view(values_);
            }

        private:
            std::vector<value_type> values_;
        };

        // particle_group
        class particle_group
        {
        public:
            // size
            sim::index size() const noexcept
            {
                return size_;
            }

            // resize
            void resize(sim::index n)
            {
                for (auto& prop : properties_) {
                    prop.second->resize(n);
                }
                size_ = n;
            }

            // require_property_array
            template<typename T>
            sim::array_view<sim::property_value_t<T>> require_property_array()
            {
                std::type_index const key = typeid(T);

                if (properties_.find(key) == properties_.end()) {
                    auto prop = std::make_unique<property<T>>();
                    prop->resize(size_);
                    properties_.emplace(key, std::move(prop));
                }

                return property_array<T>();
            }

            // property_array
            template<typename T>
            sim::array_view<sim::property_value_t<T>> property_array()
            {
                return dynamic_cast<property<T>&>(*properties_.at(typeid(T))).view();
            }

            // property_array
            template<typename T>
            sim::array_view<sim::property_value_t<T> const> property_array() const
            {
                return dynamic_cast<property<T>&>(*properties_.at(typeid(T))).view();
            }

        private:
            sim::index size_ = 0;
            std::unordered_map<std::type_index, std::unique_ptr<property_base>> properties_;
        };
    }

    // mass_property
    struct mass_property {};

    template<>
    struct property_traits<sim::mass_property> : sim::basic_property_traits<sim::scalar>
    {
        static constexpr value_type default_value() noexcept
        {
            return 1;
        }
    };

    // position_property
    struct position_property {};

    template<>
    struct property_traits<sim::position_property> : sim::basic_property_traits<sim::point>
    {
    };

    // velocity_property
    struct velocity_property {};

    template<>
    struct property_traits<sim::velocity_property> : sim::basic_property_traits<sim::vector>
    {
    };

    // basic_properties
    struct basic_properties
    {
        sim::scalar mass = property_traits<sim::mass_property>::default_value();
        sim::point position;
        sim::vector velocity;
    };

    class system;

    // forcefield
    class forcefield
    {
    public:
        virtual ~forcefield() = default;

        // compute_energy
        virtual sim::scalar compute_energy(sim::system const& system) = 0;

        // compute_force
        virtual void compute_force(sim::system const& system, sim::array_view<sim::vector> forces) = 0;
    };

    // composite_forcefield
    class composite_forcefield : public sim::forcefield
    {
    public:
        // add_component
        void add_component(std::shared_ptr<sim::forcefield> forcefield)
        {
            components_.push_back(forcefield);
        }

        // compute_energy implements sim::forcefield.
        sim::scalar compute_energy(sim::system const& system) override
        {
            sim::scalar total_energy = 0;

            for (auto& component : components_) {
                total_energy += component->compute_energy(system);
            }
            return total_energy;
        }

        // compute_force implements sim::forcefield.
        void compute_force(sim::system const& system, sim::array_view<sim::vector> forces) override
        {
            for (auto& component : components_) {
                component->compute_force(system, forces);
            }
        }

    private:
        std::vector<std::shared_ptr<sim::forcefield>> components_;
    };

    // system
    class system
    {
    public:
        system()
        {
            particles_.require_property_array<sim::mass_property>();
            particles_.require_property_array<sim::position_property>();
            particles_.require_property_array<sim::velocity_property>();
        }

        // particle_count
        sim::index particle_count() const noexcept
        {
            return particles_.size();
        }

        // add_particle
        void add_particle(sim::basic_properties const& props = {})
        {
            auto const index = particles_.size();
            particles_.resize(particles_.size() + 1);

            mass_array()[index] = props.mass;
            position_array()[index] = props.position;
            velocity_array()[index] = props.velocity;
        }

        // require_property_array
        template<typename T>
        sim::array_view<sim::property_value_t<T>> require_property_array()
        {
            return particles_.require_property_array<T>();
        }

        // property_array
        template<typename T>
        sim::array_view<sim::property_value_t<T>> property_array()
        {
            return particles_.property_array<T>();
        }

        template<typename T>
        sim::array_view<sim::property_value_t<T> const> property_array() const
        {
            return particles_.property_array<T>();
        }

        // mass_array
        sim::array_view<sim::scalar> mass_array() noexcept
        {
            return property_array<sim::mass_property>();
        }

        sim::array_view<sim::scalar const> mass_array() const noexcept
        {
            return property_array<sim::mass_property>();
        }

        // position_array
        sim::array_view<sim::point> position_array() noexcept
        {
            return property_array<sim::position_property>();
        }

        sim::array_view<sim::point const> position_array() const noexcept
        {
            return property_array<sim::position_property>();
        }

        // velocity_array
        sim::array_view<sim::vector> velocity_array() noexcept
        {
            return property_array<sim::velocity_property>();
        }

        sim::array_view<sim::vector const> velocity_array() const noexcept
        {
            return property_array<sim::velocity_property>();
        }

        // add_forcefield
        void add_forcefield(std::shared_ptr<sim::forcefield> forcefield)
        {
            forcefield_.add_component(forcefield);
        }

        // compute_energy
        sim::scalar compute_energy()
        {
            return forcefield_.compute_energy(*this);
        }

        // compute_force
        void compute_force(sim::array_view<sim::vector> forces)
        {
            return forcefield_.compute_force(*this, forces);
        }

    private:
        detail::particle_group particles_;
        sim::composite_forcefield forcefield_;
    };
}

//-------------------------------------------------------------------------------------------------

#include <iostream>


class harmonic_forcefield : public sim::forcefield
{
public:
    sim::scalar spring_constant = 0.1;

    sim::scalar compute_energy(sim::system const& system) override
    {
        sim::scalar energy = 0;
        auto positions = system.position_array();
        for (sim::index i = 0; i < system.particle_count(); i++) {
            energy += 0.5 * spring_constant * positions[i].vector().squared_norm();
        }
        return energy;
    }

    void compute_force(sim::system const& system, sim::array_view<sim::vector> forces) override
    {
        auto positions = system.position_array();
        for (sim::index i = 0; i < system.particle_count(); i++) {
            forces[i] += -spring_constant * positions[i].vector();
        }
    }
};

void force_harmonic_field(sim::system& system)
{
    system.add_forcefield(std::make_shared<harmonic_forcefield>());
}

void simulate_newtonian_dynamics(sim::system& system)
{
    int const step_count = 1000;
    sim::scalar const timestep = 0.01;
    sim::index const particle_count = system.particle_count();

    std::vector<sim::vector> forces(particle_count);

    auto positions = system.position_array();
    auto velocities = system.velocity_array();
    auto masses = system.mass_array();

    system.compute_force(ext::make_array_view(forces));

    for (int step = 0; step < step_count; step++) {
        // Velocity Verlet

        for (sim::index i = 0; i < particle_count; i++) {
            velocities[i] += timestep / (2 * masses[i]) * forces[i];
            positions[i] += timestep * velocities[i];
            forces[i] = {};
        }

        system.compute_force(ext::make_array_view(forces));

        for (sim::index i = 0; i < particle_count; i++) {
            velocities[i] += timestep / (2 * masses[i]) * forces[i];
        }
    }
}

int main()
{
    sim::system system;

    system.add_particle();
    system.add_particle({.mass = 2});
    system.add_particle({.position = {2, 3, 4}});
    system.add_particle({.mass = 10, .position = {1, 2, 3}});

    force_harmonic_field(system);

    std::cout << "Mean potential: " << system.compute_energy() / system.particle_count() << '\n';
    simulate_newtonian_dynamics(system);
    std::cout << "Mean potential: " << system.compute_energy() / system.particle_count() << '\n';

    for (auto p : system.position_array()) {
        std::cout << p << '\n';
    }
}
