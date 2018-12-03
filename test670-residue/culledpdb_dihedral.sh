#!/bin/sh -eu

culledpdb_dir="${DB_ROOT}/dunbrack.fccc.edu/Guoli/culledpdb_hh"
culledpdb="${culledpdb_dir}/cullpdb_pc90_res3.0_R1.0_d181126_chains37632.gz"

gzip -cd "${culledpdb}" |
tail -n+2               |
xargs -L1 -P32 ./culledpdb_dihedral_single.sh
