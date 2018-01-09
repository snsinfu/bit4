#include <iostream>
#include <memory>
#include <utility>

#include <OpenMM.h>

template<typename Force, typename... Args>
Force& add_force(OpenMM::System& system, Args&&... args)
{
    auto force = std::make_unique<Force>(std::forward<Args>(args)...);
    system.addForce(force.get());
    return *force.release(); // Now the pointer is managed by the system.
}

int main()
{
    OpenMM::System system;
    auto& nonbonded = add_force<OpenMM::NonbondedForce>(system);
    system.addParticle(1.0);
    nonbonded.addParticle(0.0, 0.1, 1.0);
    OpenMM::VerletIntegrator integrator(0.001);
    OpenMM::Context context(system, integrator);
    OpenMM::State state = context.getState(OpenMM::State::Positions);
    std::cout << state.getPositions().size() << '\n';
}
