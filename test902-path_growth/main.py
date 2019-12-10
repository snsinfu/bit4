import matplotlib.pyplot as plt
import numpy as np


def main():
    random = np.random.RandomState(0)
    sigma = 0.2
    wobble = 2.0
    n = 8

    path1 = generate_path(n, sigma, wobble, random)
    path2 = grow_path(path1, sigma)
    assert len(path2) == len(path1) + 1

    fig, ax = plt.subplots(figsize=(5, 5))
    ax.plot(path1[:, 0], path1[:, 1], lw=0.5, ms=2, marker="o", color="k")
    ax.plot(path2[:, 0], path2[:, 1], lw=0.5, ms=2, marker="o", color="r")
    ax.set_aspect("equal")
    plt.show(fig)


def generate_path(n, sigma, wobble, random):
    path = [np.zeros(2)]
    dirvec = np.zeros(2)
    for _ in range(1, n):
        dirvec = normalize(dirvec + wobble * random.normal(size=2))
        path.append(path[-1] + sigma * dirvec)
    return np.array(path)


def grow_path(path, sigma):
    mid = len(path) // 2
    mid_vec = sigma * normalize(path[mid + 1] - path[mid - 1])

    forward_point = path[mid] + mid_vec / 2
    backward_point = path[mid] - mid_vec / 2

    forward_path = [forward_point]
    for i in range(mid + 1, len(path)):
        forward_path.append(
            forward_path[-1] + 2 * (path[i] - forward_path[-1])
        )

    backward_path = [backward_point]
    for i in range(0, mid)[::-1]:
        backward_path.append(
            backward_path[-1] + 2 * (path[i] - backward_path[-1])
        )

    return np.array(backward_path[::-1] + forward_path)


def normalize(v):
    return v / np.linalg.norm(v)


if __name__ == "__main__":
    main()
