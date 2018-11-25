import argparse
import gzip
import math
import os
import signal

import pandas as pd
import Bio.PDB as bio


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)
    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("backbones", type=str, nargs="*", help="backbone tsvs")
    return vars(parser.parse_args())


def run(backbones):
    for backbone in backbones:
        extract_backbone_features(backbone)


def extract_backbone_features(backbone):
    try:
        backbone_table = pd.read_table(backbone)
    except:
        return

    x_axis = bio.Vector(1, 0, 0)

    seqs = backbone_table["seq"]
    coords = backbone_table.iloc[:, 2:].values

    for i in backbone_table.index[2:]:
        if not seqs[i] == seqs[i - 1] + 1 == seqs[i - 2] + 2:
            continue

        prev_res = coords[i - 2]
        cur_res = coords[i - 1]
        next_res = coords[i]

        prev_c = prev_res[9:12]
        cur_n = cur_res[0:3]
        cur_ca = cur_res[3:6]
        cur_c = cur_res[9:12]
        next_n = next_res[0:3]

        n = bio.Vector(cur_n - prev_c)
        ca = bio.Vector(cur_ca - prev_c)
        c = bio.Vector(cur_c - prev_c)
        nn = bio.Vector(next_n - prev_c)

        rot = bio.vectors.refmat(n, x_axis)
        ref_n = n.left_multiply(rot)
        ref_ca = ca.left_multiply(rot)
        ref_c = c.left_multiply(rot)
        ref_nn = nn.left_multiply(rot)

        print("{}\t{}\t{}\t{}".format(
            "\t".join(map("{:.3f}".format, ref_n)),
            "\t".join(map("{:.3f}".format, ref_ca)),
            "\t".join(map("{:.3f}".format, ref_c)),
            "\t".join(map("{:.3f}".format, ref_nn)),
        ))


if __name__ == "__main__":
    main()
