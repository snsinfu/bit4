import numpy as np


matrix = np.zeros((100, 100), dtype=np.float32)
orig_ptr = matrix.ctypes.data

# Attach metadata to the dtype. This does not make copy (astype makes one!).
matrix.dtype = np.dtype(
    matrix.dtype,
    metadata={"someint": 1234, "somedict": {"A": 0, "B": 1}}
)
assert matrix.ctypes.data == orig_ptr

# OK, we can see the metadata.
print(matrix.dtype.metadata)

# Metadata propagates in this case.
result = np.sin(matrix + matrix)
print(result.dtype.metadata)

# Not in this case due to the multiplication by a value of different type.
result = matrix * 3
print(result.dtype.metadata)

# Cast does not help!
result = matrix * matrix.dtype.type(3)
print(result.dtype.metadata)
