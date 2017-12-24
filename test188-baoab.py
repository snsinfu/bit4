# https://doi.org/10.1093/amrx/abs010
# https://arxiv.org/abs/1203.5428
#
# Langevin's equations of motion:
#
#   dq = p/M dt
#   dp = F dt - gp dt + sqrt(2MgkT) dW
#
# BAOAB scheme:
#
#   p(n+1/2) = p(n) + dt/2 F(n)
#   q(n+1/2) = q(n) + dt/2M p(n+1/2)
#   w(n+1/2) = c p(n+1/2) + sqrt(MkT(1-c^2)) R(n+1) where c = exp(-g dt)
#   q(n+1) = q(n+1/2) + dt/2M w(n+1/2)
#   p(n+1) = w(n+1/2) + dt/2 F(n+1)
#
# BAOAB limit scheme:
#
#   p(n+1) = sqrt(MkT) R(n+1)
#   q(n+1) = q(n) + dt^2/2M F(n) + dt/2M (p(n) + p(n+1))
#
# Modified timestep (dt => sqrt(2dt/g)):
#
#   p(n+1) = sqrt(MkT) R(n+1)
#   q(n+1) = q(n) + dt/M F(n) + sqrt(2dt/g) (p(n) + p(n+1))/2M
#

import matplotlib.pyplot as plt
import numpy as np

n_samples = 10000
kt = 1.0
m = 0.01
g = 100.0
dt = 0.01

def main():
    q = np.random.uniform(-1, 1, n_samples)
    p = np.zeros(n_samples)
    step = underdamped_step

    for _ in range(10000):
        step(p, q)

    plt.hist(q, bins=50)
    plt.show()

def potential(q):
    return q**4 - q**3 - q**2 + q

def force(q):
    return -4 * q**3 + 3 * q**2 + 2 * q - 1

def underdamped_step(p, q):
    c1 = np.exp(-g * dt)
    c2 = -np.expm1(-2 * g * dt)
    p += dt * force(q) / 2
    q += dt * p / (2 * m)
    p *= c1
    p += np.sqrt(m * kt * c2) * np.random.normal(size=n_samples)
    q += dt * p / (2 * m)
    p += dt * force(q) / 2

def overdamped_step(p, q):
    q += dt**2 * force(q) / (2 * m)
    q += dt * p / (2 * m)
    p[:] = np.sqrt(m * kt) * np.random.normal(size=n_samples)
    q += dt * p / (2 * m)

def modified_overdamped_step(p, q):
    q += dt * force(q) / m
    q += np.sqrt(2 * dt) * p / (2 * m)
    p[:] = np.sqrt(m * kt) * np.random.normal(size=n_samples)
    q += np.sqrt(2 * dt) * p / (2 * m)

def correct_overdamped_step(p, q):
    q += dt * force(q) / (m * g)
    q += np.sqrt(2 * dt / g) * p / (2 * m)
    p[:] = np.sqrt(m * kt) * np.random.normal(size=n_samples)
    q += np.sqrt(2 * dt / g) * p / (2 * m)

if __name__ == '__main__':
    main()
