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

    max_degree = 5
    means = np.mean(data, axis=1)

    central_moments = np.transpose([means] + [
        np.mean((data - means[:, np.newaxis])**n, axis=1) for n in range(2, max_degree + 1)
    ])

    cumulants = np.transpose([
        central_moments[:, 0],
        central_moments[:, 1],
        central_moments[:, 2],
        central_moments[:, 3] - 3 * central_moments[:, 2]**2,
        central_moments[:, 4] - 10 * central_moments[:, 2] * central_moments[:, 3]
    ])

    cumulant_features = standardize_features(cumulants)
    corr = np.corrcoef(cumulant_features)

    print_matrix(corr)


def standardize_features(data):
    return (data - np.mean(data, axis=0)[np.newaxis, :]) / np.std(data, axis=0)[np.newaxis, :]


def print_matrix(matrix):
    for row in matrix:
        print('\t'.join('{:.2f}'.format(x) for x in row))


if __name__ == '__main__':
    main()
