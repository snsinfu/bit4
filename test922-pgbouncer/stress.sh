#!/bin/sh

port=5432

pgbench -h 192.168.119.2 -p ${port} -U postgres --initialize --scale 50
pgbench -h 192.168.119.2 -p ${port} -U postgres --client 10 --transactions 500 --jobs 10
