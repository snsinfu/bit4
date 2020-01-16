import numpy as np


n = 10
sums = np.arange(n)
counts = np.arange(n)

# Here 0/0 = NaN is the intention and divide-by-zero warning is unwanted.
with np.errstate(divide="ignore", invalid="ignore"):
    means = sums / counts

assert np.isnan(means[0])
assert means[1] == 1
assert means[2] == 1
