#!/bin/sh -eu

culledpdb_dir="${DB_ROOT}/dunbrack.fccc.edu/Guoli/culledpdb_hh"
mmcif_dir="${DB_ROOT}/ftp.pdbj.org/pub/pdb/data/structures/divided/mmCIF"
culledpdb="${culledpdb_dir}/cullpdb_pc20_res3.0_R1.0_d181108_chains10300.gz"
outdir="output"

gzip -cd "${culledpdb}" |
awk 'NR >= 2 {
    id = $1

    pdb_id = tolower(substr(id, 1, length(id) - 1))
    pdb_div = substr(pdb_id, 2, 2)
    chain = substr(id, length(id))

    print id, pdb_id, pdb_div, chain
}' |
while read -r id pdb_id pdb_div chain; do
    mmcif="${mmcif_dir}/${pdb_div}/${pdb_id}.cif.gz"
    python -m backbone --chain "${chain}" "${mmcif}" > "${outdir}/${id}.tsv"
done
