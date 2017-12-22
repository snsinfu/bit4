# Estimating stddev from small sample sets drawn from a known distribution.

import numpy as np
import scipy.stats.kde as ssk

true_stddev = 1.5
sample_count = 5
trials = 1000000

def main():
    data = generate_samples()
    estimates = estimate_stddev(data)
    report_errors(estimates)

def generate_samples():
    # Stddev is not equal to sigma under lognormal distribution.
    sigma = np.sqrt(np.log((1 + np.sqrt(1 + 4 * true_stddev**2)) / 2))
    return np.random.lognormal(0, sigma, size=(trials, sample_count))

def estimate_stddev(data):
    return {'Biased': np.std(data, axis=1),
            'Bessel': np.std(data, axis=1, ddof=1),
            'Brugger': np.std(data, axis=1, ddof=1.5)}

def report_errors(estimates):
    print('Mean error:')
    report_mean_errors(estimates)
    print('\nMedian absolute error:')
    report_median_absolute_errors(estimates)
    print('\nMost likely absolute error:')
    report_likely_absolute_errors(estimates)

def report_mean_errors(estimates):
    for name, stddev in estimates.items():
        me = np.mean(stddev - true_stddev)
        print('  %s: %g' % (name, me))

def report_median_absolute_errors(estimates):
    for name, stddev in estimates.items():
        mae = np.median(np.abs(stddev - true_stddev))
        print('  %s: %g' % (name, mae))

def report_likely_absolute_errors(estimates):
    for name, stddev in estimates.items():
        mesh = np.linspace(0, 2, num=100)
        mode = likely(np.abs(stddev - true_stddev), mesh)
        print('  %s: %.2f' % (name, mode))

def likely(samples, mesh):
    kde = ssk.gaussian_kde(samples)
    density = kde(mesh)
    return mesh[np.argmax(density)]

if __name__ == '__main__':
    main()
