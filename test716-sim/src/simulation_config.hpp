#ifndef INCLUDED_SIMULATION_CONFIG_HPP
#define INCLUDED_SIMULATION_CONFIG_HPP

#include <istream>
#include <stdexcept>

#include <json.hpp>


struct simulation_config
{
    // Polymer parameters
    int    domain_length      = 10;
    int    chain_length       = 100;
    int    chain_count        = 5;
    double a_core_radius      = 0.1;
    double h_core_radius      = 0.1;
    double a_repulsive_energy = 1;
    double h_repulsive_energy = 1;
    double a_bond_strength    = 100;
    double h_bond_strength    = 100;

    // Environment parameters
    double wall_radius   = 1;
    double wall_strength = 1000;

    // Simulation parameters
    double simulation_timestep    = 1e-5;
    double simulation_temperature = 1;
    int    relaxation_steps       = 1000;
    double relaxation_spacestep   = 0.01;
    int    equilibration_steps    = 1000;
    int    sampling_count         = 10;
    int    sampling_interval      = 1000;

    // load_json creates simulation_config from a JSON string.
    static simulation_config load_json(std::istream& is);

    // validate checks parameter values. Throws an exception if some parameter
    // value is invalid.
    void validate() const;
};


inline simulation_config simulation_config::load_json(std::istream& is)
{
    auto const json = nlohmann::json::parse(is);
    auto const bind = [&](std::string section, std::string key, auto& var) {
        var = json[section][key];
    };

    simulation_config config;

#define GET(section, var) bind(section, #var, config.var);

    GET("polymer", domain_length)
    GET("polymer", chain_length)
    GET("polymer", chain_count)
    GET("polymer", a_core_radius)
    GET("polymer", h_core_radius)
    GET("polymer", a_repulsive_energy)
    GET("polymer", h_repulsive_energy)
    GET("polymer", a_bond_strength)
    GET("polymer", h_bond_strength)

    GET("environment", wall_radius)
    GET("environment", wall_strength)

    GET("simulation", simulation_timestep)
    GET("simulation", simulation_temperature)
    GET("simulation", relaxation_steps)
    GET("simulation", relaxation_spacestep)
    GET("simulation", equilibration_steps)
    GET("simulation", sampling_count)
    GET("simulation", sampling_interval)

#undef GET

    return config;
}


inline void simulation_config::validate() const
{
    auto const do_check = [](bool pred, std::string cond) {
        if (!pred) {
            throw std::runtime_error("parameter check failed: " + cond);
        }
    };

#define CHECK(pred) do_check(pred, #pred);

    CHECK(domain_length >= 1)
    CHECK(chain_length >= 1)
    CHECK(chain_count >= 1)
    CHECK(a_core_radius > 0)
    CHECK(h_core_radius > 0)
    CHECK(a_repulsive_energy >= 0)
    CHECK(h_repulsive_energy >= 0)
    CHECK(a_bond_strength >= 0)
    CHECK(h_bond_strength >= 0)

    CHECK(wall_radius > 0)
    CHECK(wall_strength >= 0)

    CHECK(simulation_timestep > 0)
    CHECK(simulation_temperature >= 0)
    CHECK(relaxation_steps >= 0)
    CHECK(relaxation_spacestep >= 0)
    CHECK(equilibration_steps >= 0)
    CHECK(sampling_count >= 0)
    CHECK(sampling_interval >= 1)

#undef CHECK
}


#endif
