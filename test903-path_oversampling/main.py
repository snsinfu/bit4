
import matplotlib.pyplot as plt
import numpy as np


def main():
    random = np.random.RandomState(0)
    sigma = 0.2
    wobble = 2.0
    n = 8
    mul = 10

    path1 = generate_path(n, sigma, wobble, random)
    path2 = oversample_path(path1, mul)
    assert len(path2) == len(path1) * mul

    fig, ax = plt.subplots(figsize=(5, 5))
    ax.plot(path1[:, 0], path1[:, 1], lw=0.5, ms=3, marker="o", color="k")
    ax.plot(path2[:, 0], path2[:, 1], lw=0.1, ms=1, marker="o", color="r")
    ax.set_aspect("equal")
    plt.show(fig)


def generate_path(n, sigma, wobble, random):
    path = [np.zeros(2)]
    dirvec = np.zeros(2)
    for _ in range(1, n):
        dirvec = normalize(dirvec + wobble * random.normal(size=2))
        path.append(path[-1] + sigma * dirvec)
    return np.array(path)


def oversample_path(path, n):
    fine_path = []
    prv = None
    cur = None
    nxt = path[0]
    for i in range(len(path)):
        prv, cur = cur, nxt

        if i + 1 < len(path):
            nxt = path[i + 1]
        else:
            nxt = None

        if prv is None:
            prv_vec = cur - nxt
        else:
            prv_vec = prv - cur

        if nxt is None:
            nxt_vec = cur - prv
        else:
            nxt_vec = nxt - cur

        prv_seg = np.linspace(-0.5, 0, num=(n // 2), endpoint=False)
        prv_seg = cur - prv_seg[:, None] * prv_vec[None, :]

        nxt_seg = np.linspace(0, 0.5, num=(n // 2), endpoint=False)
        nxt_seg = cur + nxt_seg[:, None] * nxt_vec[None, :]

        fine_path.extend(prv_seg)
        fine_path.extend(nxt_seg)

    return np.array(fine_path)


def normalize(v):
    return v / np.linalg.norm(v)


if __name__ == "__main__":
    main()
