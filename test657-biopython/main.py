import gzip
import json
import os
import warnings

import pandas as pd
import Bio.PDB as bio


DB_ROOT = os.getenv("DB_ROOT")
CULLEDPDB_DIR = DB_ROOT + "/dunbrack.fccc.edu/Guoli/culledpdb_hh"
MMCIF_DIR = DB_ROOT + "/ftp.pdbj.org/pub/pdb/data/structures/divided/mmCIF"

culledpdb_path = CULLEDPDB_DIR + "/cullpdb_pc20_res3.0_R1.0_d181108_chains10300.gz"
culledpdb = pd.read_table(culledpdb_path, sep="\s+")

entry = culledpdb.iloc[0]
culledpdb_id = entry["IDs"]
resolution = entry["resolution"]
r_free = entry["FreeRvalue"]

pdb_id = culledpdb_id[:-1].lower()
chain_id = culledpdb_id[-1]

mmcif_path = MMCIF_DIR + "/" + pdb_id[1:3] + "/" + pdb_id + ".cif.gz"
parser = bio.MMCIFParser()
with gzip.open(mmcif_path, "rt") as file:
    structure = parser.get_structure(pdb_id, file)

metadata = {
    "pdb_id": pdb_id,
    "chain": chain_id,
    "resolution": resolution,
    "r_free": r_free,
}

print("# " + json.dumps(metadata))
print("seq\tname\tN_x\tN_y\tN_z\tCA_x\tCA_y\tCA_z\tC_x\tC_y\tC_z")

chain = structure[0][chain_id]
for res in chain:
    if not bio.is_aa(res):
        continue
    hetflag, resseq, icode = res.id
    print("{}\t{}\t{}\t{}\t{}".format(
        resseq,
        res.resname,
        "\t".join(map("{:.3f}".format, res["N"].get_vector())),
        "\t".join(map("{:.3f}".format, res["CA"].get_vector())),
        "\t".join(map("{:.3f}".format, res["C"].get_vector())),
    ))
