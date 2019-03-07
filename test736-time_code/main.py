"""
Using sin(p * rate * t) and cos(p * rate * t) for encoding time information.
"""
import numpy as np


def main():
    time = np.linspace(-1, 1, num=30)
    prime = np.array([2, 3, 5, 7, 11, 13, 17, 19])
    rate = 0.5
    phase = rate * np.multiply.outer(time, prime)
    time_code = np.hstack([np.sin(phase), np.cos(phase)])

    # Emulate some "activation"
    act = np.hstack([
        sigmoid(+0.7 * time_code + 0.0),
        sigmoid(+0.5 * time_code - 0.2),
        sigmoid(-0.9 * time_code + 0.7),
    ])

    # To see a distinct time code pattern!
    for a in act:
        row = "".join(" ."[x] for x in np.round(a).astype(int))
        print(row)


def sigmoid(x):
    return 1 / (1 + np.exp(-x))


if __name__ == "__main__":
    main()
