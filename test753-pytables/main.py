# THIS SCRIPT DOES NOT WORK

import numpy as np
import tables


def main():
    with tables.open_file("a.h5", mode="w") as store:
        build(store)


def build(store):
    column_shape = (10000,)
    description = {
        "feature_a": tables.Int16Col(shape=column_shape),
        "feature_b": tables.Int16Col(shape=column_shape),
        "feature_c": tables.Int16Col(shape=column_shape),
        "label": tables.Int8Col(shape=column_shape),
    }

    group = store.create_group(store.root, "foo")
    table = store.create_table(group, "tab", description, expectedrows=column_shape[0])

    feature_a = np.random.randint(12345, size=column_shape)
    feature_b = np.random.randint(23456, size=column_shape)
    feature_c = np.random.randint(34567, size=column_shape)
    label = ((feature_b - feature_a) > feature_c)

    table.modify_column(colname="feature_a", column=feature_a)
    table.modify_column(colname="feature_b", column=feature_b)
    table.modify_column(colname="feature_c", column=feature_c)
    table.modify_column(colname="label", column=label)

    store.flush()


if __name__ == "__main__":
    main()
