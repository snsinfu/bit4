"""
Extract dihedral features from an mmCIF file.

Usage:
  dihedral.py [-m <index>] [-c <id>] <mmcif>
  dihedral.py -h

Options:
  -m, --model <index>  Specify model index [default: 0]
  -c, --chain <id>     Specify chain ID [default: A]
  -h, --help           Print this help message and exit
"""
import gzip
import math
import signal

import Bio.PDB as bio
import docopt

__version__ = "0.1.0"

DEGREES_PER_RADIAN = 180 / math.pi


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    opts = docopt.docopt(__doc__, version=__version__)
    mmcif = opts["<mmcif>"]
    model = int(opts["--model"])
    chain = opts["--chain"]

    run(mmcif, model, chain)


def run(mmcif, model, chain):
    structure = load_mmcif(mmcif)
    backbone = extract_backbone(structure[model][chain])

    for pre, cur, nex in zip(backbone[:-2], backbone[1:-1], backbone[2:]):
        if not pre.seq + 2 == cur.seq + 1 == nex.seq:
            continue

        phi = bio.vectors.calc_dihedral(pre.c, cur.n, cur.ca, cur.c)
        psi = bio.vectors.calc_dihedral(cur.n, cur.ca, cur.c, nex.n)
        std_n, std_ca, std_c, std_nn = standardize_coords(pre, cur, nex)

        fields = [
            "{:.1f}".format(phi * DEGREES_PER_RADIAN),
            "{:.1f}".format(psi * DEGREES_PER_RADIAN),
            "\t".join(map("{:.3f}".format, std_n)),
            "\t".join(map("{:.3f}".format, std_ca - std_n)),
            "\t".join(map("{:.3f}".format, std_c - std_ca)),
            "\t".join(map("{:.3f}".format, std_nn - std_c)),
        ]
        print("\t".join(fields))


def standardize_coords(pre, cur, nex):
    x_axis = bio.Vector(1, 0, 0)
    y_axis = bio.Vector(0, 1, 0)

    # Define prev C = origin
    po = pre.o - pre.c
    n = cur.n - pre.c
    ca = cur.ca - pre.c
    c = cur.c - pre.c
    nn = nex.n - pre.c

    # Define C-N = x-axis
    rot_x = bio.vectors.refmat(n, x_axis)

    po = po.left_multiply(rot_x)
    n = n.left_multiply(rot_x)
    ca = ca.left_multiply(rot_x)
    c = c.left_multiply(rot_x)
    nn = nn.left_multiply(rot_x)

    # Define cross(C-N,C=O) = y-axis
    rot_y = bio.vectors.refmat(n ** po, y_axis)

    std_n = n.left_multiply(rot_y)
    std_ca = ca.left_multiply(rot_y)
    std_c = c.left_multiply(rot_y)
    std_nn = nn.left_multiply(rot_y)

    return std_n, std_ca, std_c, std_nn


class ResidueBackbone:
    def __init__(self, res):
        self.res = res
        self.seq = res.id[1]
        self.n = res["N"].get_vector()
        self.ca = res["CA"].get_vector()
        self.c = res["C"].get_vector()
        self.o = res["O"].get_vector()


def extract_backbone(chain):
    """
    Extract (res, n, ca, c, o) for each amino acid residue in given chain. res
    is the residue object and n, ca, c and o are the coordinate vectors of
    backbone atoms. Illegular residues are skipped.
    """
    backbone = []
    for res in chain:
        if not bio.is_aa(res):
            continue
        try:
            backbone.append(ResidueBackbone(res))
        except:
            pass
    return backbone


def load_mmcif(path):
    """
    Load an mmCIF file. It decompresses the file if the path ends with ".gz".
    """
    parser = bio.MMCIFParser()

    if path.endswith(".gz"):
        with gzip.open(path, "rt") as file:
            return parser.get_structure("", file)

    return parser.get_structure("", path)


if __name__ == "__main__":
    main()
