import argparse
import gzip
import math
import os
import signal

import Bio.PDB as bio


HEADER = "seq\tname\tN_x\tN_y\tN_z\tCA_x\tCA_y\tCA_z\tCB_x\tCB_y\tCB_z\tC_x\tC_y\tC_z"


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)
    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--structure", type=str, default=0, help="structure index")
    parser.add_argument("--chain", type=str, default="A", help="chain id")
    parser.add_argument("cif", type=str, help="mmcif file")
    return vars(parser.parse_args())


def run(cif, structure, chain):
    mmcif = load_mmcif(cif)
    backbone = generate_backbone(mmcif[structure][chain])

    print(HEADER)
    for res, n, ca, cb, c in backbone:
        _, seq, _ = res.id
        print(format_row(seq, res.resname, n, ca, cb, c))


def generate_backbone(chain):
    """
    Generates (res, n, ca, cb, c) for each amino acid residue in given chain
    where res is the residue object and n, ca, cb and c are the coordinate
    vectors of the N, CA, CB and C atoms.
    """
    for res in chain:
        if not bio.is_aa(res):
            continue

        try:
            n = res["N"].get_vector()
            ca = res["CA"].get_vector()
            c = res["C"].get_vector()

            if "CB" in res:
                cb = res["CB"].get_vector()
            else:
                cb = compute_virtual_cbeta(n, ca, c)

            yield res, n, ca, cb, c
        except:
            pass


def compute_virtual_cbeta(n, ca, c):
    """
    Compute virtual C-beta coordinates from N, CA and C coordinates.
    """
    v_n = n - ca
    v_c = c - ca
    rot = bio.rotaxis(math.pi * 120 / 180, v_c)
    v_cb = delta_n.left_multiply(rot)
    return ca + v_cb


def load_mmcif(path):
    """
    Load an mmCIF file. It decompresses the file if the path ends with ".gz".
    """
    parser = bio.MMCIFParser()

    if path.endswith(".gz"):
        with gzip.open(path, "rt") as file:
            return parser.get_structure("", file)

    return parser.get_structure("", path)


def format_row(seq, name, n_vec, ca_vec, cb_vec, c_vec):
    """
    Format given data as tab-separated row consistent with the HEADER.
    """
    return "{}\t{}\t{}\t{}\t{}\t{}".format(
        seq,
        name,
        "\t".join(map("{:.3f}".format, n_vec)),
        "\t".join(map("{:.3f}".format, ca_vec)),
        "\t".join(map("{:.3f}".format, cb_vec)),
        "\t".join(map("{:.3f}".format, c_vec)),
    )


if __name__ == "__main__":
    main()
