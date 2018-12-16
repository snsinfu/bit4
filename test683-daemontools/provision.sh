#!/bin/sh

sudo pkg install -qy daemontools mime-support icecast2

mkdir -p service
cp -r /vagrant/icecast service
chmod u+x service/icecast/run
crontab /vagrant/crontab

sudo service cron restart
