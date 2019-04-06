import tables
import numpy as np


def main():
    data = make_data()

    with tables.open_file("data.h5", "w") as storage:

        # raw               12 MB
        # zlib              451 KB
        # lzo               1.2 MB
        # blosc:blosclz     1.7 MB
        # blosc:zlib        441 KB
        # blosc:zstd        311 KB

        compression = tables.Filters(complib="blosc:zstd", complevel=5)

        stored_data = storage.create_carray(
            storage.root,
            "data",
            atom=tables.Int16Atom(),
            shape=data.shape,
            filters=compression,
        )
        stored_data[:] = data


def make_data():
    t = np.arange(1000000)
    p = np.multiply.outer(t, [2, 3, 5, 7, 11, 13])
    return np.round(100 * np.sin(p) ** 2).astype(np.int16)


if __name__ == "__main__":
    main()
