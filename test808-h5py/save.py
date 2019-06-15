import json
import time

import h5py
import numpy as np


with h5py.File("data.h5", "w") as store:
    # h5py deduces float type when attrs["creation_time"] = time.time() is
    # used. So manually create the attribute with explicit type info.
    store.attrs.create("creation_time", time.time(), dtype=int)

    # String is correctly stored as string.
    metadata = {"foo": 1.23, "bar": 256}
    store.attrs["metadata"] = json.dumps(metadata, indent=2)

    # Generate some compressible data.
    data = np.zeros((100, 5))
    for i in range(1, data.shape[0]):
        data[i, :-1] += data[i - 1, 1:]
        data[i, -1] = np.random.uniform(-1, 1)

    # Quantize data to help compression. Data is not converted to int as it
    # has only minor effect (~15% or so); the shuffle filter does a good job
    # transforming float bits to a compressible form.
    scale = 2 ** 15
    data = np.round(data * scale) / scale

    dataset_opts = dict(
        chunks=True, shuffle=True, compression="gzip", compression_opts=6
    )
    dataset = store.create_dataset("timepoints/0", data.shape, **dataset_opts)
    dataset[:] = data
