#!/bin/sh
#PBS -N mpi
#PBS -l nodes=2:ppn=4
cd "${PBS_O_WORKDIR}"
mpirun -np "${PBS_NP}" --hostfile "${PBS_NODEFILE}" ./main
