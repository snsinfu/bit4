#
# |n + s R|
#

import matplotlib.pyplot as plt
import numpy as np


dimension = 3
sample_count = 30000
max_sigma = 10

e = np.zeros(dimension)
e[0] = 1

sigmas = np.random.uniform(0, max_sigma, size=sample_count)
vectors = e + sigmas[:, np.newaxis] * np.random.normal(0, 1, size=(sample_count, dimension))
norms = np.linalg.norm(vectors, axis=1)
fits_a = np.sqrt(1 + 3 * sigmas**2)
fits_b = np.sqrt(1 + 2.55 * sigmas**2)

plt.scatter(sigmas, norms, s=1, lw=0, color='gray')
plt.scatter(sigmas, fits_a, s=1, lw=0, color='r')
plt.scatter(sigmas, fits_b, s=1, lw=0, color='b')
plt.show()
