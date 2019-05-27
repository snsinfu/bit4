import argparse

import numpy as np
import tables


def main():
    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("filename", type=str)
    parser.add_argument("path", type=str)
    return vars(parser.parse_args())


def run(filename, path):
    with tables.open_file(filename) as store:
        node = store.root
        for name in path.split("/"):
            node = node[name]

        print("Node:")
        print(node)

        print("Data:")
        if isinstance(node, tables.Leaf):
            print("")
            print(node[:])


if __name__ == "__main__":
    main()
