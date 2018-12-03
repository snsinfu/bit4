"""
Extract backbone coordinates from an mmCIF file.

Usage:
  backbone.py [-s <index>] [-c <id>] <mmcif>
  backbone.py -h

Options:
  -s, --structure <index>  Specify structure index [default: 0]
  -c, --chain <id>         Specify chain ID [default: A]
  -h, --help               Print this help message and exit
"""
import gzip
import signal

import Bio.PDB as bio
import docopt

__version__ = "0.1.0"


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    opts = docopt.docopt(__doc__, version=__version__)
    mmcif = opts["<mmcif>"]
    structure = int(opts["--structure"])
    chain = opts["--chain"]

    run(mmcif, structure, chain)


def run(mmcif, structure, chain):
    entry = load_mmcif(mmcif)
    backbone = generate_backbone(entry[structure][chain])
    output_backbone(backbone)


def output_backbone(backbone):
    """
    Write backbone data to stdout in tabular format.
    """
    print(HEADER)

    for res, n, ca, c, o in backbone:
        _, seq, _ = res.id
        print(format_row(seq, res.resname, n, ca, c, o))


HEADER = "{}\t{}\t{}\t{}\t{}\t{}".format(
    "seq",
    "name",
    "\t".join(map("N_{}".format, "xyz")),
    "\t".join(map("CA_{}".format, "xyz")),
    "\t".join(map("C_{}".format, "xyz")),
    "\t".join(map("O_{}".format, "xyz")),
)


def format_row(seq, name, n, ca, c, o):
    """
    Format single residue backbone data in the format consistent with HEADER.
    """
    return "{}\t{}\t{}\t{}\t{}\t{}".format(
        seq,
        name,
        "\t".join(map("{:.3f}".format, n)),
        "\t".join(map("{:.3f}".format, ca)),
        "\t".join(map("{:.3f}".format, c)),
        "\t".join(map("{:.3f}".format, o)),
    )


def generate_backbone(chain):
    """
    Generates (res, n, ca, c, o) for each amino acid residue in given chain.
    res is the residue object and n, ca, c and o are the coordinate vectors of
    backbone atoms. Illegular residues are skipped.
    """
    for res in chain:
        if not bio.is_aa(res):
            continue

        try:
            n = res["N"].get_vector()
            ca = res["CA"].get_vector()
            c = res["C"].get_vector()
            o = res["O"].get_vector()

            yield res, n, ca, c, o
        except:
            pass


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
