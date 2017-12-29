import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d
import numpy as np
import simtk.openmm.openmm as mm
import simtk.unit

def main():
    num_particles = 13
    particle_mass = 1.0
    particle_diameter = 1.0
    lennard_jones_epsilon = 5 * 2.5
    temperature = 0.0
    friction = 1.0e5
    timestep = 1.0e-3
    initial_positions = np.random.uniform(-1.0, 1.0, size=(num_particles, 3))
    num_steps = 10000

    system = mm.System()

    nonbond = mm.NonbondedForce()
    nonbond.setNonbondedMethod(mm.NonbondedForce.NoCutoff)
    for _ in range(num_particles):
        system.addParticle(mass=particle_mass)
        nonbond.addParticle(charge=0, sigma=particle_diameter, epsilon=lennard_jones_epsilon)
    system.addForce(nonbond)

    integrator = mm.LangevinIntegrator(temperature, friction, timestep)

    context = mm.Context(system, integrator)
    context.setPositions(initial_positions)
    context.setVelocitiesToTemperature(temperature)

    integrator.step(num_steps)

    state = context.getState(getPositions=True)
    positions = state.getPositions(asNumpy=True).value_in_unit(simtk.unit.nanometers)
    bound = np.max(np.abs(positions))

    fig = plt.figure()
    ax = fig.add_subplot(111, aspect='equal', projection='3d')
    ax.scatter(positions[:, 0], positions[:, 1], positions[:, 2],
               c=np.arange(len(positions)), cmap='rainbow')
    ax.set_xlim(-bound, bound)
    ax.set_ylim(-bound, bound)
    ax.set_zlim(-bound, bound)
    plt.show()

if __name__ == '__main__':
    main()
