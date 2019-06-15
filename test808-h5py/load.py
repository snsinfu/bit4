import json

import h5py
import numpy as np


with h5py.File("data.h5", "r") as store:
    print("creation_time:", store.attrs["creation_time"])
    print("metadata:", json.loads(store.attrs["metadata"]))

    for step in store["timepoints"]:
        dataset = store["timepoints"][step]
        data = dataset[:]

print("last row:", data[-1])
