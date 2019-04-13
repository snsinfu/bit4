import numpy as np
import tables


filename = "a.h5"
sample_count = 10
category_count = 4
feature_count = 3
alpha = 2.0


def main():
    np.random.seed(0)

    with tables.open_file(filename, mode="w") as store:
        build(store)

    with tables.open_file(filename) as store:
        read(store)


def build(store):
    category = np.random.randint(category_count, size=sample_count)
    features = np.random.standard_gamma(alpha, size=(sample_count, feature_count))

    category_store = store.create_carray(
        store.root, "category", atom=tables.Int8Atom(), shape=category.shape
    )
    category_store[:] = category

    features_store = store.create_carray(
        store.root, "features", atom=tables.Float32Atom(), shape=features.shape
    )
    features_store[:] = features


def read(store):
    category = store.root["category"]
    features = store.root["features"]
    indices = np.arange(features.shape[0])

    category_mask = np.array([True] * category_count)
    category_mask[0] = False

    mask1 = category_mask[category[:]]
    mask2 = features[:, 0] < 3

    print(mask1.astype(int))
    print(mask2.astype(int))
    print((mask1 & mask2).astype(int))

    masked_indices = indices[mask1 & mask2]
    print("")
    print(features[masked_indices, :])


if __name__ == "__main__":
    main()
