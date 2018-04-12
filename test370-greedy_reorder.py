import sys

import numpy as np


def main():
    reference_points = np.array([
        [1, 0, 0, 0],
        [0, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1],
    ])

    points = np.array([
        [0.5, 1.9, 0.2, 0.3],
        [0.1, 1.8, 6.5, 1.1],
        [1.5, 0.6, 0.1, 3.4],
        [0.8, 0.1, 0.2, 0.1],
    ])

    epsilon = 0.01

    def similarity(u, v):
        p = np.clip(u / np.sum(u), epsilon, 1 - epsilon)
        q = np.clip(v / np.sum(v), epsilon, 1 - epsilon)
        return np.dot(p, q)

    permutation = deduce_permutation(reference_points, points, similarity)
    reordered_points = points[permutation]

    np.savetxt(sys.stdout,
               reordered_points,
               fmt='%.2g',
               delimiter='\t')


def deduce_permutation(src, dest, score):
    if len(src) != len(dest):
        raise ValueError('lengths mismatch')
    count = len(src)

    permutation = np.arange(count)
    for i, src_point in enumerate(src):
        for k in range(i, count):
            score_value = score(src_point, dest[permutation[k]])
            if k == i or score_value > best_score:
                best_score = score_value
                best_index = k
        swap(permutation, i, best_index)

    return permutation


def swap(arr, i, j):
    arr[i], arr[j] = arr[j], arr[i]


if __name__ == '__main__':
    main()
