#!/bin/sh -eu
cd $(dirname $0)
distfile="hello-2.10.tar.gz"
curl -LO "https://ftp.gnu.org/gnu/hello/${distfile}"
tar --strip-components 1 -xf ${distfile}
