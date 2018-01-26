# Hilbert-Schmidt Independence Criterion

import numpy as np
import scipy.stats as stats
import sklearn.metrics.pairwise as smp

def main():
    x = np.linspace(-10, 10, num=500)
    y = np.cos(5 * x)

    display_correlation('Pearson', *stats.pearsonr(x, y))
    display_correlation('Spearman', *stats.spearmanr(x, y))
    display_correlation('HSIC', *hsic(x, y, sigma_x=0.1, sigma_y=0.1))

def display_correlation(name, corr, pvalue, alpha=0.01):
    if pvalue < alpha:
        decision = 'correlated'
    else:
        decision = 'independent'
    print('{}: {} (pvalue = {:.3g})'.format(name, decision, pvalue))

def hsic(X, Y, sigma_x, sigma_y):
    K = rbf_grammian(X, sigma_x)
    L = rbf_grammian(Y, sigma_y)
    return kernel_hsic(K, L)

def rbf_grammian(X, sigma):
    X = standardize_data_array(X)
    return smp.rbf_kernel(X, X, 0.5 / sigma**2)

def standardize_data_array(X):
    X = np.asarray(X)
    if len(X.shape) == 1:
        X = X[:, None]
    return X

def kernel_hsic(K, L):
    if K.shape != L.shape:
        raise Exception('shape mismatch')

    if not is_square(K):
        raise Exception('square matrix is expected')

    n = K.shape[0]

    # Make grammians centered
    HK = K - K.mean(axis=0)[:, None]
    HL = L - L.mean(axis=0)[:, None]
    HKH = HK - HK.mean(axis=0)[None, :]
    HLH = HL - HL.mean(axis=0)[None, :]

    # Calculate the HSIC statistic. Note that trace(dot(HKH, HLH)) is just
    # the sum of the element-wise product of the two matrices.
    HSIC = np.dot(HKH.reshape(-1), HLH.reshape(-1)) / n**2

    # Estimate the expectation value of HSIC under the null hypothesis that
    # the two datasets are mutually independent.
    mu2_x = K.sum() / (n * (n - 1))
    mu2_y = L.sum() / (n * (n - 1))
    expect_HSIC = (1 + mu2_x * mu2_y - mu2_x - mu2_y) / n

    # Estimate the variance of HSIC under the null hypothesis.
    B = (HKH * HLH)**2
    Cxx_Cyy = (B.sum() - B.trace()) / (n * (n - 1))
    var_HSIC = 2 * (n - 4) * (n - 5) * Cxx_Cyy / (n * (n - 1) * (n - 2) * (n - 3))

    # n * HSIC is gamma-distributed under the null hypothesis.
    if expect_HSIC > 0:
        stat = n * HSIC
        shape = expect_HSIC**2 / var_HSIC
        scale = n * var_HSIC / expect_HSIC
        pvalue = stats.gamma.sf(stat, shape, scale=scale)
    else:
        # expect_HSIC can be zero or negative due to estimation bias when
        # the true expectation value is very small. In such cases we cannot
        # test the HSIC value.
        pvalue = None

    return HSIC, pvalue

def is_square(X):
    return len(X.shape) == 2 and X.shape[0] == X.shape[1]

if __name__ == '__main__':
    main()
