"""
Estimate the parameters of a multinomial distribution with Dirichlet prior:

    x ~ Multinomial(p),   p ~ Dirichlet(a) .

MAP estimate (if ai >= 1 for all i):

    pi = (xi + ai - 1) / (n + (a1 + ... + ak) - k) ,   i = 1,...,k .

Expected value:

    pi = (xi + ai) / (n + a1 + ... + ak) ,   i = 1,...,k .

"""


def main():
    samples = [
        (259, 24, 18, 38, 11),
        (20, 86, 81, 140, 489),
        (29, 12, 70, 90, 115),
        (12, 140, 27, 30, 8),
        (12, 210, 27, 35, 4),
    ]
    alpha = 1.5

    for sample in samples:
        distr = estimate_multinomial_parameter(sample, alpha)
        print_distribution(distr)


def estimate_multinomial_parameter(sample, alpha):
    distr = [x + alpha - 1.0 for x in sample]
    norm = 0.0
    for p in distr:
        norm += p
    return [p / norm for p in distr]


def print_distribution(distr):
    print('\t'.join('%.2f' % p for p in distr))


if __name__ == '__main__':
    main()
