#!/bin/sh
set -eu

id user1 2>&1 > /dev/null || sudo adduser -m -g users -u 1101 user1
id user2 2>&1 > /dev/null || sudo adduser -m -g users -u 1102 user2

# Create files with incorrect UIDs. This simulates UID migration.
mkdir -p data
mkdir -p data/user1
mkdir -p data/user2

touch data/user1/a.tsv
touch data/user2/b.tsv

sudo chown -R 2001 data/user1
sudo chown -R 2002 data/user2

# Now, migrate UIDs
sudo chown -hR --from 2001 1101 data/user1
sudo chown -hR --from 2002 1102 data/user2

# Test
test "$(find data -user user1)" = "\
data/user1
data/user1/a.tsv"

test "$(find data -user user2)" = "\
data/user2
data/user2/b.tsv"

echo OK
