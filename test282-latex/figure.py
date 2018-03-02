import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(-10, 10, num=1000)
y = np.sin(x) / (x + 1e-6)

fig = plt.figure(figsize=(5, 3), tight_layout={'rect': (0, 0, 1, 0.95)}, dpi=72)

ax = fig.add_subplot(1, 1, 1)
ax.plot(x, y)

fig.suptitle('Lorem ipsum')
fig.savefig('ipsum.pdf')
