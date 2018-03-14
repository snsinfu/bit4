import collections
import itertools
import math
import random


sample_density = 1.0
sample_count = 100
bin_width = 0.25


def main():
    print('Uniform sampling:')
    show_histogram(generate_linspace)

    print('\nPoisson process sampling:')
    show_histogram(generate_poisson_process)


def show_histogram(sampling_method):
    points = sampling_method(sample_density)
    responses = (math.sin(math.pi * t) for t in points)
    binned_responses = (round(x / bin_width) for x in responses)

    samples = itertools.islice(binned_responses, 0, sample_count)
    counter = collections.Counter(samples)

    for x in sorted(counter):
        print('{:3.0f}: {:.2f}'.format(x, float(counter[x]) / sample_count))


def generate_linspace(density):
    t = 0
    while True:
        yield t
        t += 1 / density


def generate_poisson_process(density):
    t = 0
    while True:
        t += random.expovariate(density) # or -log(uniform(0, 1)) / density
        yield t


if __name__ == '__main__':
    main()
