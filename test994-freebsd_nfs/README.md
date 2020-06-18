# NFSv4 numeric uid:gid on FreeBSD server

When NFSv4 server is on FreeBSD numeric gid sent by a Linux client seems to be
not recognized by the FreeBSD server. When the server is Linux the numeric gid
is handled as expected. So I believe this should be a configuration problem.

## sysctl

`man nfsv4` says this:

> The form where the numbers are in the strings can only be used for
> AUTH_SYS.  To configure your systems this way, the nfsuserd(8) daemon
> does not need to be running on the server, but the following sysctls need
> to be set to 1 on the server.
```
      vfs.nfs.enable_uidtostring
      vfs.nfsd.enable_stringtouid
```

Setting these sysctls did not solve the issue.

## uid/gid wire format incompatibility?

I searched FreeBSD src for "stringtouid" and found this:

https://github.com/freebsd/freebsd/blob/release/12.1.0/sys/fs/nfs/nfs_commonsubs.c#L3187

```c
	/* If a string of digits and an AUTH_SYS mount, just convert it. */
	str0 = str;
	tuid = (uid_t)strtoul(str0, &endstr, 10);
	if ((endstr - str0) == len) {
		/* A numeric string. */
		...
```

So, FreeBSD expects NFS user name (and group name similarly) to be completely
made of uid digits, not in the form "uid@domain". Linux client might be sending
uid/gid in the latter form.

## packet capture

Captured packets on FreeBSD server while creating file on Linux client.

```console
$ sudo tcpdump -i em1 -w p.log
$ tcpdump -r p.log -XX | less
```

The dump showed textual uid "4613" but no gid "4512". So it's not sent with
OPEN command. Strangely, I could fix the gid of the file on the Linux client
using chown and FreeBSD recognized the change:

```
$ uname
FreeBSD

$ ls -l /srv/share
total 1
-rw-r--r--  1 4613  4512  0 Jun 17 17:45 alice.txt
```

So, gid is neither sent or set _on file creation_ when FreeBSD NFS server is
used. It would have been understandable as a server issue if gid were sent
because gid is sent and set if Linux NFS server is used. However, gid is not
sent at all. WTF?

## nfsv3

I captured packets on both FreeBSD server and Linux server (test993) and found
out that the Linux server used NFSv3. Below is a tshark inspection on Linux. It
shows NFSv3 RPCs. uid and gid are sent as numeric values.

```
=== client to server ===

Network File System, CREATE Call DH: 0x62d40c52/hoge.txt Mode: UNCHECKED
    [Program Version: 3]
    [V3 Procedure: CREATE (8)]
    where
        dir
            length: 8
            [hash (CRC-32): 0x62d40c52]
            FileHandle: 0100010000000000
        Name: hoge.txt
            length: 8
            contents: hoge.txt
    Create Mode: UNCHECKED (0)
    obj_attributes
        mode: value follows
            set_it: value follows (1)
            Mode: 0644, S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH
                .... .... .... .... .... 0... .... .... = S_ISUID: No
                .... .... .... .... .... .0.. .... .... = S_ISGID: No
                .... .... .... .... .... ..0. .... .... = S_ISVTX: No
                .... .... .... .... .... ...1 .... .... = S_IRUSR: Yes
                .... .... .... .... .... .... 1... .... = S_IWUSR: Yes
                .... .... .... .... .... .... .0.. .... = S_IXUSR: No
                .... .... .... .... .... .... ..1. .... = S_IRGRP: Yes
                .... .... .... .... .... .... ...0 .... = S_IWGRP: No
                .... .... .... .... .... .... .... 0... = S_IXGRP: No
                .... .... .... .... .... .... .... .1.. = S_IROTH: Yes
                .... .... .... .... .... .... .... ..0. = S_IWOTH: No
                .... .... .... .... .... .... .... ...0 = S_IXOTH: No
        uid: no value
            set_it: no value (0)
        gid: no value
            set_it: no value (0)
        size: no value
            set_it: no value (0)
        atime: don't change
            set_it: don't change (0)
        mtime: don't change
            set_it: don't change (0)

=== server to client ===

Network File System, CREATE Reply
    [Program Version: 3]
    [V3 Procedure: CREATE (8)]
    Status: NFS3_OK (0)
    obj
        handle_follow: value follows (1)
        handle
            length: 16
            [hash (CRC-32): 0xdfa05a8b]
            FileHandle: 010001010000000007ff0100062dac03
    obj_attributes  Regular File mode: 0644 uid: 4613 gid: 4512
        attributes_follow: value follows (1)
        attributes  Regular File mode: 0644 uid: 4613 gid: 4512
            Type: Regular File (1)
            Mode: 0644, S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH
                .... .... .... .... .... 0... .... .... = S_ISUID: No
                .... .... .... .... .... .0.. .... .... = S_ISGID: No
                .... .... .... .... .... ..0. .... .... = S_ISVTX: No
                .... .... .... .... .... ...1 .... .... = S_IRUSR: Yes
                .... .... .... .... .... .... 1... .... = S_IWUSR: Yes
                .... .... .... .... .... .... .0.. .... = S_IXUSR: No
                .... .... .... .... .... .... ..1. .... = S_IRGRP: Yes
                .... .... .... .... .... .... ...0 .... = S_IWGRP: No
                .... .... .... .... .... .... .... 0... = S_IXGRP: No
                .... .... .... .... .... .... .... .1.. = S_IROTH: Yes
                .... .... .... .... .... .... .... ..0. = S_IWOTH: No
                .... .... .... .... .... .... .... ...0 = S_IXOTH: No
            nlink: 1
            uid: 4613
            gid: 4512
            size: 0
            used: 0
            rdev: 0,0
                specdata1: 0
                specdata2: 0
            fsid: 0x0000000000000000 (0)
            fileid: 130823
            atime: Jun 18, 2020 15:55:55.230053746 GMT
                seconds: 1592495755
                nano seconds: 230053746
            mtime: Jun 18, 2020 15:55:55.230053746 GMT
                seconds: 1592495755
                nano seconds: 230053746
            ctime: Jun 18, 2020 15:55:55.230053746 GMT
                seconds: 1592495755
                nano seconds: 230053746
```

Below is from FreeBSD. NFSv4 is used. Linux client only sends 24-byte "owner"
in OPEN opcode and no group information is sent. This is understandable as
NFSv4 OPEN operation does not accept owner_group (as noted in test975). So,
this is a protocol limitation I think.

```
=== client to server ===

Network File System, Ops(5): PUTFH, OPEN, GETFH, ACCESS, GETATTR
    [Program Version: 4]
    [V4 Procedure: COMPOUND (1)]
    Tag: <EMPTY>
        length: 0
        contents: <EMPTY>
    minorversion: 0
    Operations (count: 5): PUTFH, OPEN, GETFH, ACCESS, GETATTR
        Opcode: PUTFH (22)
            FileHandle
                length: 28
                [hash (CRC-32): 0xf51ab691]
                FileHandle: eae2d3e4defc20520a005ca100000000fb1d000000000000<E2><80><A6>
        Opcode: OPEN (18)
            seqid: 0x00000005
            share_access: OPEN4_SHARE_ACCESS_WRITE (2)
            share_deny: OPEN4_SHARE_DENY_NONE (0)
            clientid: 0x9439ea5e02000000
            owner: <DATA>
                length: 24
                contents: <DATA>
            Open Type: OPEN4_CREATE (1)
                Create Mode: UNCHECKED4 (0)
                Attr mask: 0x00000002 (Mode)
                    reco_attr: Mode (33)
                        mode: 0644, Name: Unknown, Read permission for owner, Write permission for owner, Read permission for group, Read permission for others
                            .... .... .... .... 000. .... .... .... = Name: Unknown (0)
                            .... .... .... .... .... 0... .... .... = Set user id on exec: No
                            .... .... .... .... .... .0.. .... .... = Set group id on exec: No
                            .... .... .... .... .... ..0. .... .... = Save swapped text even after use: No
                            .... .... .... .... .... ...1 .... .... = Read permission for owner: Yes
                            .... .... .... .... .... .... 1... .... = Write permission for owner: Yes
                            .... .... .... .... .... .... .0.. .... = Execute permission for owner: No
                            .... .... .... .... .... .... ..1. .... = Read permission for group: Yes
                            .... .... .... .... .... .... ...0 .... = Write permission for group: No
                            .... .... .... .... .... .... .... 0... = Execute permission for group: No
                            .... .... .... .... .... .... .... .1.. = Read permission for others: Yes
                            .... .... .... .... .... .... .... ..0. = Write permission for others: No
                            .... .... .... .... .... .... .... ...0 = Execute permission for others: No
            Claim Type: CLAIM_NULL (0)
                Name: foo.txt
                    length: 7
                    contents: foo.txt
                    fill bytes: opaque data
        Opcode: GETFH (10)
        Opcode: ACCESS (3), [Check: RD MD XT XE]
            Check access: 0x2d
                .... ...1 = 0x001 READ: allowed?
                .... .1.. = 0x004 MODIFY: allowed?
                .... 1... = 0x008 EXTEND: allowed?
                ..1. .... = 0x020 EXECUTE: allowed?
        Opcode: GETATTR (9)
            Attr mask[0]: 0x0010011a (Type, Change, Size, FSID, FileId)
                reqd_attr: Type (1)
                reqd_attr: Change (3)
                reqd_attr: Size (4)
                reqd_attr: FSID (8)
                reco_attr: FileId (20)
            Attr mask[1]: 0x00b0a23a (Mode, NumLinks, Owner, Owner_Group, RawDev, Space_Used, Time_Access, Time_Metadata, Time_Modify, Mounted_on_FileId)
                reco_attr: Mode (33)
                reco_attr: NumLinks (35)
                reco_attr: Owner (36)
                reco_attr: Owner_Group (37)
                reco_attr: RawDev (41)
                reco_attr: Space_Used (45)
                reco_attr: Time_Access (47)
                reco_attr: Time_Metadata (52)
                reco_attr: Time_Modify (53)
                reco_attr: Mounted_on_FileId (55)
    [Main Opcode: OPEN (18)]

=== server to client ===

Network File System, Ops(5): PUTFH OPEN GETFH ACCESS GETATTR
    [Program Version: 4]
    [V4 Procedure: COMPOUND (1)]
    Status: NFS4_OK (0)
    Tag: <EMPTY>
        length: 0
        contents: <EMPTY>
    Operations (count: 5)
        Opcode: PUTFH (22)
            Status: NFS4_OK (0)
        Opcode: OPEN (18)
            Status: NFS4_OK (0)
            StateID
                [StateID Hash: 0x0999]
                StateID seqid: 1
                StateID Other: 9439ea5e020000000b000000
                [StateID Other hash: 0x485f8db5]
            change_info
                Atomic: No
                changeid (before): 8013398
                changeid (after): 8013399
            result flags: 0x00000004, locktype posix
                .... .... .... .... .... .... .... ..0. = confirm: False
                .... .... .... .... .... .... .... .1.. = locktype posix: True
                .... .... .... .... .... .... .... 0... = preserve unlinked: False
                .... .... .... .... .... .... ..0. .... = may notify lock: False
            Attr mask: 0x00000002 (Mode)
                reco_attr: Mode (33)
            Delegation Type: OPEN_DELEGATE_NONE (0)
        Opcode: GETFH (10)
            Status: NFS4_OK (0)
            Filehandle
                length: 28
                [hash (CRC-32): 0x2e0ba133]
                FileHandle: eae2d3e4defc20520a00201201000000b963000000000000<E2><80><A6>
        Opcode: ACCESS (3), [Access Denied: XE], [Allowed: RD MD XT]
            Status: NFS4_OK (0)
            Supported types (of requested): 0x2d
                .... ...1 = 0x001 READ: supported
                .... .1.. = 0x004 MODIFY: supported
                .... 1... = 0x008 EXTEND: supported
                ..1. .... = 0x020 EXECUTE: supported
            Access rights (of requested): 0x0d
                .... ...1 = 0x001 READ: allowed
                .... .1.. = 0x004 MODIFY: allowed
                .... 1... = 0x008 EXTEND: allowed
                ..0. .... = 0x020 EXECUTE: *Access Denied*
        Opcode: GETATTR (9)
            Status: NFS4_OK (0)
            Attr mask[0]: 0x0010011a (Type, Change, Size, FSID, FileId)
                reqd_attr: Type (1)
                    ftype4: NF4REG (1)
                reqd_attr: Change (3)
                    changeid: 8013395
                reqd_attr: Size (4)
                    size: 0
                reqd_attr: FSID (8)
                    fattr4_fsid
                        fsid4.major: 3839091434
                        fsid4.minor: 1377893598
                reco_attr: FileId (20)
                    fileid: 70176
            Attr mask[1]: 0x00b0a23a (Mode, NumLinks, Owner, Owner_Group, RawDev, Space_Used, Time_Access, Time_Metadata, Time_Modify, Mounted_on_FileId)
                reco_attr: Mode (33)
                    mode: 0644, Name: Unknown, Read permission for owner, Write permission for owner, Read permission for group, Read permission for others
                        .... .... .... .... 000. .... .... .... = Name: Unknown (0)
                        .... .... .... .... .... 0... .... .... = Set user id on exec: No
                        .... .... .... .... .... .0.. .... .... = Set group id on exec: No
                        .... .... .... .... .... ..0. .... .... = Save swapped text even after use: No
                        .... .... .... .... .... ...1 .... .... = Read permission for owner: Yes
                        .... .... .... .... .... .... 1... .... = Write permission for owner: Yes
                        .... .... .... .... .... .... .0.. .... = Execute permission for owner: No
                        .... .... .... .... .... .... ..1. .... = Read permission for group: Yes
                        .... .... .... .... .... .... ...0 .... = Write permission for group: No
                        .... .... .... .... .... .... .... 0... = Execute permission for group: No
                        .... .... .... .... .... .... .... .1.. = Read permission for others: Yes
                        .... .... .... .... .... .... .... ..0. = Write permission for others: No
                        .... .... .... .... .... .... .... ...0 = Execute permission for others: No
                reco_attr: NumLinks (35)
                    numlinks: 1
                reco_attr: Owner (36)
                    fattr4_owner: 1000
                        length: 4
                        contents: 1000
                reco_attr: Owner_Group (37)
                    fattr4_owner_group: 0
                        length: 1
                        contents: 0
                        fill bytes: opaque data
                reco_attr: RawDev (41)
                    specdata1: 4294966812
                    specdata2: 283574368
                reco_attr: Space_Used (45)
                    space_used: 512
                reco_attr: Time_Access (47)
                    seconds: 1592495410
                    nseconds: 899996000
                reco_attr: Time_Metadata (52)
                    seconds: 1592495410
                    nseconds: 899996000
                reco_attr: Time_Modify (53)
                    seconds: 1592495410
                    nseconds: 899996000
                reco_attr: Mounted_on_FileId (55)
                    fileid: 0x0000000000011220
    [Main Opcode: OPEN (18)]
```
