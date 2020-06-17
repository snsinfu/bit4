import argparse

args = [
    "--value", "1",
    "--value", "2",
    "--value", "3",
]

parser = argparse.ArgumentParser()
parser.add_argument("--value", type=int, default=[], action="append")
options = vars(parser.parse_args(args))

print(options)
assert options == {"value": [1, 2, 3]}
