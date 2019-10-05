#!/bin/sh
#PBS -N mpi
#PBS -l nodes=4:ppn=4
cd "${PBS_O_WORKDIR}"
network="192.168.108.0/24"
mpirun                                       \
  -np        "${PBS_NP}"                     \
  --hostfile "${PBS_NODEFILE}"               \
  --mca      btl_tcp_if_include "${network}" \
  ./main
