# For a unit vector n, a smallish positive number s and a normally distributed
# three-dimensional vector R,
#
#     E[|n + s R|] ~ sqrt(1 + 2.535 s^2) .
#
# For very large s the following would work better:
#
#     E[|n + s R|] ~ sqrt(8/pi) s ~ sqrt(2.546479) s ~ 1.595769 s.
#

import matplotlib.pyplot as plt
import numpy as np

from sklearn.linear_model import LinearRegression


dimension = 3
sigma_sample_count = 10000
sigma_bin_count = 1000
max_sigma = 5.0
min_sigma = 0.0001

e = np.zeros(dimension)
e[0] = 1

sigmas = np.linspace(min_sigma, max_sigma, num=sigma_bin_count)
normals = np.random.normal(0, 1, size=(sigma_bin_count, sigma_sample_count, dimension))
vectors = e + sigmas[:, np.newaxis, np.newaxis] * normals
norms = np.linalg.norm(vectors, axis=2)
mean_norms = np.mean(norms, axis=1)

#print(np.min(mean_norms))
#
#linreg = LinearRegression()
#x = np.log(sigmas)
#y = np.log(mean_norms**2 - 1 + 1e-6)
#linreg.fit(x[:, np.newaxis], y, sample_weight=sigmas)
#exponent = linreg.coef_[0]
#scale = np.exp(linreg.intercept_)
#print(exponent, scale)
#fits = np.sqrt(1 + scale * sigmas**exponent)

#linreg = LinearRegression(fit_intercept=False)
#x = np.transpose([sigmas, sigmas**2])
#y = mean_norms**2 - 1
#linreg.fit(x, y)
#a, b = linreg.coef_
#print(a, b)
#fits = np.sqrt(1 + a * sigmas + b * sigmas**2)

linreg = LinearRegression(fit_intercept=False)
x = (sigmas**2)[:, np.newaxis]
y = mean_norms**2 - 1
linreg.fit(x, y)
a, = linreg.coef_
print(a)
fits = np.sqrt(1 + a * sigmas**2)

plt.scatter(sigmas, mean_norms, s=1, lw=0, color='gray')
plt.scatter(sigmas, fits, s=1, lw=0, color='r')

plt.show()
