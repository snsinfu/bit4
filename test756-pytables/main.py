import numpy as np
import pandas as pd
import tables


def main():
    with tables.open_file("a.h5", mode="w") as store:
        build(store)

    with tables.open_file("a.h5") as store:
        read(store)


def build(store):
    features = np.random.randint(12345, size=(10000, 3))
    label = (features[:, 0] + features[:, 1] > features[:, 2])

    foo_group = store.create_group(store.root, "foo")
    bar_group = store.create_group(foo_group, "bar")
    compression = tables.Filters(complib="blosc:zstd", complevel=5)

    features_store = store.create_carray(
        bar_group,
        "features",
        atom=tables.Int16Atom(),
        shape=features.shape,
        filters=compression,
    )
    label_store = store.create_carray(
        bar_group,
        "label",
        atom=tables.Int8Atom(),
        shape=label.shape,
        filters=compression,
    )

    features_store[:] = features
    label_store[:] = label


def read(store):
    features = store.get_node("/foo/bar/features")
    label = store.get_node("/foo/bar/label")
    indices = np.arange(features.shape[0])

    selected_indices = indices[label[:] == 0]
    selected_data = features[selected_indices, :]
    print(type(selected_data))  # In-memory ndarray. Beware the size!
    print(selected_data.shape)


if __name__ == "__main__":
    main()
