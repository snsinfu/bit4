import numpy as np
import pandas as pd
import tables


def main():
    size = 10000

    store = pd.HDFStore("a.h5")

    feature_a = np.random.randint(12345, size=size, dtype=np.int16)
    feature_b = np.random.randint(23456, size=size, dtype=np.int16)
    feature_c = np.random.randint(13579, size=size, dtype=np.int16)
    label = ((feature_b - feature_a) > feature_c).astype(np.int8)

    table = pd.DataFrame.from_dict({
        "feature_a": feature_a,
        "feature_b": feature_b,
        "feature_c": feature_c,
        "label": label
    })

    table.to_hdf(
        store, key="/foo/bar", format="table", complib="blosc:zstd", complevel=5
    )


if __name__ == "__main__":
    main()
