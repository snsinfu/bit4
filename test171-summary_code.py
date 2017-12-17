import codecs
import operator

def sum_summary(numbers):
    factors = [
        205, 40, 107, 93, 32, 140, 116, 82, 47, 54, 80, 93, 230, 238, 233, 195,
        139, 239, 249, 136, 107, 208, 253, 123, 50, 203, 117, 139, 230, 237,
        247, 83, 231, 157, 79, 127, 76, 196, 142, 27, 208, 103, 75, 5, 88, 217,
        35, 128, 254, 188, 29, 68, 134, 165, 45, 158, 22, 102, 25, 49, 252, 200,
        128, 49,
    ]

    weights = []
    for number, factor in zip(numbers, factors):
        weights.append(number * factor)

    summary = []
    for i in range(len(weights)):
        summary.append(sum(weights[::(i + 1)]) % 256)
    return bytes(summary)

def hexstr(bs):
    return codecs.encode(bs, 'hex').decode('ascii')

sha = '01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b'
raw_bits = codecs.decode(sha, 'hex')
summary_bits = sum_summary(raw_bits)

print(hexstr(sum_summary([0x12, 0x34, 0x56, 0x78])))
print(hexstr(sum_summary([0x12, 0x34, 0x56, 0x79])))
