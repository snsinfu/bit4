import numpy as np


def main():
    data = [
        # Normal distribution
        [ 1.250, -0.913,  1.570, -1.020, -0.938,  0.777,  0.543, -1.270 ],
        [ 0.217,  0.229,  0.219, -0.371, -0.921,  2.320, -0.181,  0.351, -0.398,  1.470 ],
        [ -1.21,  1.630,  0.829,  0.447, -0.637,  0.603, -1.62,   1.710, -1.490, -0.0791 ],
        [-0.504,  1.110, -0.109, -1.280, -0.401,  0.958, -0.922, -0.500, -0.182 ],

        # Exponential distribution
        [ 0.712, 1.300, 0.152, 2.1500, 0.0437,  0.124, 0.234,  1.39 ],
        [ 1.570, 0.019, 2.320, 0.0467, 3.98,    0.489, 0.0535, 0.00513, 1.87 ],
        [ 0.343, 0.290, 3.210, 0.0794, 0.00256, 0.64,  0.213,  2.25,    2.16 ]
    ]

    distance_matrix = np.zeros((len(data), len(data)))

    for i in range(distance_matrix.shape[0]):
        for j in range(distance_matrix.shape[1]):
            distance_matrix[i, j] = maximum_mean_discrepancy(data[i], data[j])

    pcs, evs, _ = pca(distance_matrix)
    print_matrix(pcs)


def maximum_mean_discrepancy(x, y, sigma=1):
    dxx = np.subtract.outer(x, x)
    dxy = np.subtract.outer(x, y)
    dyy = np.subtract.outer(y, y)

    mmd_xx = np.mean(np.exp(-dxx**2 / (2 * sigma)))
    mmd_xy = np.mean(np.exp(-dxy**2 / (2 * sigma)))
    mmd_yy = np.mean(np.exp(-dyy**2 / (2 * sigma)))

    return mmd_xx + mmd_yy - 2 * mmd_xy


def pca(data):
    data = data - np.mean(data, axis=0)[np.newaxis, 0]
    u, s, vh = np.linalg.svd(data, full_matrices=False)

    f = data.shape[0] - 1
    pcs = u * np.sqrt(f)
    evs = s**2 / f
    axes = vh

    return pcs, evs, axes


def print_matrix(matrix):
    for row in matrix:
        print('\t'.join('{:.2f}'.format(x) for x in row))


if __name__ == '__main__':
    main()
