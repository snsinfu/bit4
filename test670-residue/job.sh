#!/bin/sh
#PBS -N cull
#PBS -l nodes=1:ppn=32

pipenv run sh ./culledpdb_dihedral.sh
