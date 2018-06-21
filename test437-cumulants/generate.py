import signal
import sys

import numpy as np


batch_size = 1000


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    generators_map = {
        'uniform':     generate_uniform,
        'normal':      generate_normal,
        'lognormal':   generate_lognormal,
        'exponential': generate_exponential
    }

    if len(sys.argv) != 2:
        sys.stderr.write('specify generator as an argument\n')
        sys.exit(1)

    name = sys.argv[1]

    if name not in generators_map:
        sys.stderr.write('unrecognized generator\n')
        sys.exit(1)

    for value in generators_map[name]():
        print('{:.3g}'.format(value))


def generate_uniform():
    while True:
        for value in np.random.uniform(size=batch_size):
            yield value


def generate_normal():
    while True:
        for value in np.random.normal(size=batch_size):
            yield value


def generate_lognormal():
    while True:
        for value in np.random.lognormal(size=batch_size):
            yield value


def generate_exponential():
    while True:
        for value in np.random.exponential(size=batch_size):
            yield value


if __name__ == '__main__':
    main()
