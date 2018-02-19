FROM debian:stretch-slim

RUN export DEBIAN_FRONTEND=noninteractive \
 && echo exit 101 > /usr/sbin/policy-rc.d \
 && apt-get update -q                     \
 && apt-get install -q -y dovecot-imapd   \
 && rm -rf /var/lib/apt/lists/*           \
 && useradd -m alice                      \
 && echo alice:alice | chpasswd

EXPOSE 143 993

CMD ["/usr/sbin/dovecot", "-F"]
