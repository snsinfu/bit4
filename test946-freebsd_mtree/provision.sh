#!/bin/sh -eu

mtree -eu << END
/set uname=vagrant gname=vagrant
.
    ssh-ish                 type=dir  mode=0700
        authorized_keys     type=file mode=0600
    ..
END
