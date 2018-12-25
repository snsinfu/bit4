import zlib

import msgpack


with open("example.msgpack", "rb") as file:
    data = msgpack.unpackb(zlib.decompress(file.read()))
    print(f"OK: length = {len(data)}")
