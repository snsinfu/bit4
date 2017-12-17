import codecs
import functools
import operator

def xor_summary(numbers):
    summary = []
    for i in range(len(numbers)):
        summary.append(functools.reduce(operator.xor, numbers[::(i + 1)]))
    return summary

def hexstr(bs):
    return codecs.encode(bs, 'hex').decode('ascii')

sha = '01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b'
raw_bits = codecs.decode(sha, 'hex')
summary_bits = bytes(xor_summary(raw_bits))

print(hexstr(raw_bits))
print(hexstr(summary_bits))

# Encoding and decoding
a = [0x12, 0x34, 0x56, 0x78]

s = [a[0] ^ a[1] ^ a[2] ^ a[3],
     a[0]        ^ a[2],
     a[0]               ^ a[3],
     a[0]]

b = [s[3],
     s[3] ^ s[2] ^ s[1] ^ s[0],
     s[3] ^ s[1],
     s[3] ^ s[2]]

assert b == a

# The above encoding is a linear transformation on Z/2Z
#
#         [ 1 1 1 1 ]
#     T = [ 1 0 1 0 ]
#         [ 1 0 0 1 ]
#         [ 1 0 0 0 ]
#
# which has the inverse
#
#         [ 0 0 0 1 ]
#     T = [ 1 1 1 1 ]
#         [ 0 1 0 1 ]
#         [ 0 0 1 1 ]

# Sensitive to local changes?
print(hexstr(bytes(xor_summary([0x12, 0x34, 0x56, 0x78]))))
print(hexstr(bytes(xor_summary([0x12, 0x34, 0x56, 0x10]))))

# Not really...
