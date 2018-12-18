#!/bin/sh

sudo pkg install -qy runit mime-support icecast2 tmux htop
sudo ln -s /usr/local/etc/mime.types /etc

cp -r /vagrant/service .
cp /vagrant/user_login.conf .login_conf
crontab /vagrant/user_crontab

sudo service cron restart
