import tables


enum = tables.Enum([
    "foo",
    "bar",
    "baz"
])

assert enum["bar"] == 1
assert type(enum["bar"]) is int
assert enum(1) == "bar"
