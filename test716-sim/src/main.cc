#include <exception>
#include <fstream>
#include <iostream>

#include <md.hpp>


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

class simulation_driver
{
public:
    explicit simulation_driver(simulation_config const&)
    {
    }

    void run()
    {
    }
};


int main()
{
    try {
        simulation_config config;

        if (std::ifstream file{"config.json"}) {
            config = load_simulation_config(file);
        }
        config.validate();

        simulation_driver driver{config};
        driver.run();
    } catch (std::exception const& e) {
        std::cerr << "error: " << e.what() << std::endl;
        throw;
    }
}
