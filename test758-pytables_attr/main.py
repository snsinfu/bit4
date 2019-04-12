import numpy as np
import tables

with tables.open_file("a.h5", mode="w") as store:
    data = np.random.randint(-10000, 10000, size=(100000, 3))
    columns = ["A", "B", "C"]

    data_store = store.create_carray(
        store.root,
        "data",
        shape=data.shape,
        atom=tables.Int16Atom(),
        filters=tables.Filters(complib="blosc:zstd", complevel=5),
    )
    data_store[:] = data
    data_store.set_attr("columns", columns)

    # Note: Looks like the attribute is serialized with pickle protocol 0.
