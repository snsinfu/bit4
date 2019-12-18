#!/bin/sh
pgbench -h 192.168.119.2 -p 6432 -U postgres --initialize --scale 50
pgbench -h 192.168.119.2 -p 6432 -U postgres --client 10 --transactions 1000 --jobs 2
