#!/bin/sh -eu

outdir="output"
mmcif_dir="${DB_ROOT}/ftp.pdbj.org/pub/pdb/data/structures/divided/mmCIF"

echo "$1" |
awk '{
    id = $1

    pdb_id = tolower(substr(id, 1, length(id) - 1))
    pdb_div = substr(pdb_id, 2, 2)
    chain = substr(id, length(id))

    print id, pdb_id, pdb_div, chain
}' |
while read -r id pdb_id pdb_div chain; do
    mmcif="${mmcif_dir}/${pdb_div}/${pdb_id}.cif.gz"
    output="${outdir}/${id}.tsv"

    if [ -f "${output}" ]; then
        echo "${id}: skipped" >&2
        continue
    else
        echo "${id}: processing..." >&2
    fi

    python -m dihedral --chain "${chain}" "${mmcif}" > "${output}" || rm "${output}"
done
