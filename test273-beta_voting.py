def main():
    cases = [
        [0.5, 0.5, 0.5],
        [0.3, 0.6, 0.6],
        [0.1, 0.5, 0.7],
        [0.8, 0.8, 0.8],
        [0.1, 0.1, 0.1],
        [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9],
    ]
    for probs in cases:
        mean = sum(probs) / len(probs)
        beta = vote(probs, beta=0.5)
        print('{:g}\t{:g}'.format(mean, beta))


def vote(probs, beta=1, eps=1e-6):
    # https://stats.stackexchange.com/a/188587/90293
    #
    # Assume that the probability distribution, x, is conditionally beta
    # distributed with hyperparameters a and b:
    #
    #           { Beta(a + b, a)     if z = 0
    #   x | z ~ {
    #           { Beta(a, a + b)     if z = 1
    #
    # or
    #
    #              { G(2a+b) / (G(a) G(a+b)) x^(a-1) (1-x)^(a+b-1)  if z = 0,
    #   p(x | z) = {
    #              { G(2a+b) / (G(a+b) G(a)) x^(a+b-1) (1-x)^(a-1)  if z = 1.
    #
    # Here G is the gamma function. Now, with the uniform prior z ~ Uniform
    # and observed data X = (x1, ..., xn), the Bayes' theorem gives
    #
    #   p(z=1 | X) = p(X | z=1) p(z=1) / p(X)
    #
    #              = 1 / (1 + \prod_i^n (1/xi - 1)^b) .
    #
    # The hyperparameter b controls how strongly the posterior is affected by
    # the observed data; b = 0 always produces p(z=1 | X) = 0.5. The other
    # hyperparameter a is ancillary.
    #
    x = 1.0
    for p in probs:
        p = min(1 - eps, max(eps, p))
        x *= (1.0 / p - 1)**beta
    return 1 / (1 + x)


if __name__ == '__main__':
    main()
