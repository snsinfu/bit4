#!/bin/sh -eu

culledpdb_dir="${DB_ROOT}/dunbrack.fccc.edu/Guoli/culledpdb_hh"
culledpdb="${culledpdb_dir}/cullpdb_pc20_res3.0_R1.0_d181108_chains10300.gz"

gzip -cd "${culledpdb}" |
tail -n+2 |
xargs -L1 -P16 ./culledpdb_backbone_single.sh
