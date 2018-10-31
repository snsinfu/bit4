import gzip

import msgpack
import numpy as np


with gzip.open("trajectory.pack.gz") as file:
    pack = msgpack.unpack(file, encoding="utf-8")

    metadata = pack["metadata"]
    frames = pack["frames"]

    print(f"title: {metadata['title']}")
    print(f"id: {metadata['id']}")

    inverse_multiplier = 1 / metadata["multiplier"]

    for n, frame in enumerate(frames):
        # Decode fixed-point number
        frame = np.array(frame) * inverse_multiplier

        # Do some analysis
        print("#{}: mean={:.4f} std={:.4f}".format(n, np.mean(frame), np.std(frame)))
