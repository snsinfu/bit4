Packer inserts a temporary key, which is registered to root's authorized\_key
file by the cloud-init service. Thus, if I have launched a new server based on
the image, root's authorized\_key file would contain an obsolete key entry.

In this test613 experiment, the packer provisioner is configured to remove the
temporary authorized\_key file in the created image. It correctly gets rid of
the obsolete key entry in a newly laucnhed server.

To run, set HCLOUD\_TOKEN and type these commands:

```console
make
make ssh
make clean
```
