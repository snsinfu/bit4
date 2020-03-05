Set Hetzner Cloud token:

```
export HCLOUD_TOKEN=...
```

Launch a server:

```
ansible-playbook infra_up.yml
```

Provision the server:

```
ansible-playbook provision.yml
```

Now you have `_wireguard_client.conf` generated. Fill in the PrivateKey field
with the following value:

```
PrivateKey = gMsCWrQznJpL6c5+aTgrPYZRphGCqSNgTA/Bovju+kc=
```

(This is for demo purpose. In actual deployment you should generate private
public keypair in a client machine and only copy the public key into
config/variables.yml.)

After provisioning the server can only be accessed via VPN. To ssh into the
server you first need to enable wireguard tunnel on your client.
