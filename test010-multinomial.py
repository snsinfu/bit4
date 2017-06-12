import numpy as np
import scipy.special as special

prob = np.array([1, 2, 3, 4, 5], dtype=float)
prob /= prob.sum()

k = 10000
sample = np.random.multinomial(k, prob)

# xi log pi
log_likelihood = (sample * np.log(prob)).sum()
log_likelihood += special.gammaln(sample.sum() + 1) - (special.gammaln(sample + 1)).sum()
print(log_likelihood)

