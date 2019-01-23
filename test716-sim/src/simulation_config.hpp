#ifndef INCLUDED_SIMULATION_CONFIG_HPP
#define INCLUDED_SIMULATION_CONFIG_HPP

#include <istream>


struct simulation_config
{
    void validate()
    {
    }
};

simulation_config load_simulation_config(std::istream&)
{
    return {};
}

#endif
