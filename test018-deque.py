from collections import deque
from itertools import chain

def rolling_sum(xs, win):
    q = deque(maxlen=win)
    offset = win // 2
    for x in chain(xs, [0] * offset):
        q.appendleft(x)
        if offset > 0:
            offset -= 1
        else:
            yield sum(q)

xs = [1, 2, 3, 4, 5, 6, 7]
ys = rolling_sum(xs, 3)

print(list(ys))
