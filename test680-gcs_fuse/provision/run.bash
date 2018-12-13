#!/bin/bash
set -eux -o pipefail

srcdir="$(dirname $(realpath $0))"

# Install gcsfuse
sudo tee /etc/apt/sources.list.d/gcsfuse.list > /dev/null << EOF
deb http://packages.cloud.google.com/apt gcsfuse-$(lsb_release -cs) main
EOF
curl -fsSL https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -

export DEBIAN_NONINTERACTIVE=true
sudo apt-get update -qq
sudo apt-get install -qq -y gcsfuse

# Enable background user services
loginctl enable-linger

# Install GCS auto-mount service
install -d -m 0755 ~/.config/systemd/user
install -C -m 0644 ${srcdir}/bucket.service ~/.config/systemd/user
install -d -m 0755 ~/bucket

systemctl --user enable bucket
systemctl --user start bucket
