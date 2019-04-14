import numpy as np
import tables


filename = "a.h5"
sample_count = 10
category_count = 4
feature_count = 10
alpha = 2.0


def main():
    np.random.seed(0)

    with tables.open_file(filename, mode="w") as store:
        build(store)

    with tables.open_file(filename) as store:
        read(store)


def build(store):
    data = np.random.standard_gamma(alpha, size=(sample_count, feature_count))

    data_store = store.create_carray(
        store.root, "data", atom=tables.Float32Atom(), shape=data.shape
    )
    data_store[:] = data


def read(store):
    data = store.root["data"]
    indices = np.arange(data.shape[0])
    mask = data[:, 0] < 3

    masked_indices = indices[mask]
    column_indices = [1, 3, 5, 7, 9]

    # IndexError: Only one selection list is allowed
    # print(data[masked_indices, column_indices])

    # OK
    print(data[masked_indices, :][:, column_indices])


if __name__ == "__main__":
    main()
