# FreeBSD NFS server/client

Testing FreeBSD server/client pair to see if the uid:gid issue is caused by
FreeBSD/Linux incompability on some NFS corner case. The result is negative.
The gid of newly created file is 0 even if the client is FreeBSD.

But! I found that if I first create a directory, fix the gid and then create
a new file in the directory, the new file is correctly owned by the expected
uid:gid. Now I think that the issue is related to the sticky bit set to the
shared directory. Maybe FreeBSD and Linux differ in the handling of sticky bit.
