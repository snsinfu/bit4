#!/bin/sh

sudo pkg install -qy daemontools mime-support icecast2
sudo ln -s /usr/local/etc/mime.types /etc

mkdir -p service
cp -r /vagrant/icecast service
crontab /vagrant/crontab

sudo service cron restart
