import numpy as np
import tables


def main():
    with tables.open_file("a.h5", mode="w") as store:
        build(store)


def build(store):
    description = {
        "feature_a": tables.Int16Col(),
        "feature_b": tables.Int16Col(),
        "feature_c": tables.Int16Col(),
        "label": tables.Int8Col(),
    }
    size = 10000

    group = store.create_group(store.root, "foo")
    table = store.create_table(group, "tab", description, expectedrows=size)

    feature_a = np.random.randint(12345, size=size)
    feature_b = np.random.randint(23456, size=size)
    feature_c = np.random.randint(34567, size=size)
    label = ((feature_b - feature_a) > feature_c)

    for a, b, c, l in zip(feature_a, feature_b, feature_c, label):
        row = table.row
        row["feature_a"] = a
        row["feature_b"] = b
        row["feature_c"] = c
        row["label"] = l
        row.append()

    store.flush()


if __name__ == "__main__":
    main()
