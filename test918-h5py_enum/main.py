import h5py
import numpy as np

types = np.arange(50) // 10
types_map = {
    "apple": 0, "banana": 1, "cherry": 2, "durian": 3, "elderberry": 4
}

with h5py.File("_a.h5", "w") as store:
    # Does not work. No setter for dtype.
    # store["types"] = types
    # store["types"].dtype = h5py.special_dtype(enum=("i", types_map))

    # Works.
    store.create_dataset(
        "types", data=types, dtype=h5py.special_dtype(enum=(np.int8, types_map))
    )

    # Note: h5py >= 2.10.0 deprecates `special_dtype` in favor of `enum_dtype`.
