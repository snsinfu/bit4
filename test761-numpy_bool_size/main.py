import numpy as np


n = 15478399
a = np.ones(n, dtype=np.bool)
print("{:.2f} MiB".format(a.nbytes / 1024 ** 2))

b = np.packbits(a)
print("{:.2f} MiB".format(b.nbytes / 1024 ** 2))
