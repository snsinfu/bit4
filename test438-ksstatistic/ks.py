import numpy as np


def main():
    data = np.array([
        # Normal distribution
        [ 1.25, -0.913, 1.57, -1.02, -0.938, 0.777, 0.543, -1.27, -0.465, -0.145 ],
        [ 0.217, 0.229, 0.219, -0.371, -0.921, 2.32, -0.181, 0.351, -0.398, 1.47 ],
        [ -0.504, 1.11, -0.353, -0.109, -1.28, -0.401, 0.958, -0.922, -0.5, -0.182 ],
        # Exponential distribution
        [ 0.712, 1.3, 0.152, 2.15, 0.0437, 0.124, 0.234, 1.39, 1.49, 0.00968 ],
        [ 1.57, 0.0192, 2.32, 0.0467, 3.98, 0.489, 0.066, 0.0535, 0.00513, 1.87 ],
        [ 0.343, 0.29, 3.21, 0.0794, 0.00256, 0.64, 0.415, 0.213, 2.25, 2.16 ]
    ])

    distance_matrix = np.zeros((len(data), len(data)))

    for i in range(distance_matrix.shape[0]):
        for j in range(distance_matrix.shape[1]):
            distance_matrix[i, j] = kolmogorov_smirnov(data[i], data[j])

    print_matrix(distance_matrix)


def kolmogorov_smirnov(x, y):
    x = np.sort(x)
    y = np.sort(y)
    space = np.concatenate((x, y))

    x_cdf = np.searchsorted(x, space).astype(float) / len(x)
    y_cdf = np.searchsorted(y, space).astype(float) / len(y)

    return np.max(np.abs(x_cdf - y_cdf))


def print_matrix(matrix):
    for row in matrix:
        print('\t'.join('{:.1f}'.format(x) for x in row))


if __name__ == '__main__':
    main()
