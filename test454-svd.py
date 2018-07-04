import numpy as np


original_matrix = np.array([
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9],
])

u, s, vh = np.linalg.svd(original_matrix)

n = s.size
pcs = u * np.sqrt(n - 1)
evs = s**2 / (n - 1)
axes = vh

whitened_matrix = np.multiply.outer(pcs[:, 0], axes[0, :])
reconstructed_matrix = np.sqrt(evs[0]) * whitened_matrix

print(whitened_matrix)
