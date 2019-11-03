# https://docs.ansible.com/ansible/latest/user_guide/vault.html#vault-payload-format-1-1-1-2

from binascii import hexlify, unhexlify


def split(s):
    return [line.strip() for line in s.split()]


with open("foo") as file:
    vault = file.read()

header, *hex_payload = split(vault)
hex_payload = "".join(hex_payload)
payload = unhexlify(hex_payload).decode("ascii")

hex_salt, hex_hmac, *hex_ciphertext = split(payload)
hex_ciphertext = "".join(hex_ciphertext)

salt = unhexlify(hex_salt)
ciphertext = unhexlify(hex_ciphertext)

print("salt")
print(f"{len(salt)} bytes")
print(salt)

print("\nciphertext")
print(f"{len(ciphertext)} bytes")
print(ciphertext)
