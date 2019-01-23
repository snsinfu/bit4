#include <exception>
#include <fstream>
#include <iostream>

#include <md.hpp>

#include "simulation_config.hpp"
#include "simulation_driver.hpp"


int main()
{
    simulation_config config;

    if (std::ifstream file{"config.json"}) {
        config = load_simulation_config(file);
    }
    config.validate();

    simulation_driver driver{config};
    driver.run();
}
