import msgpack

with open("example.msgpack", "rb") as file:
    msg = msgpack.load(file, encoding="utf-8")

print(msg)
