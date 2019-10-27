### Periodic backup

Watch minio data directory and see how backups are created every minute and
old backups are deleted every 5 minutes.

```
watch ls -hlt /srv/minio/data/db-backups/main/basebackups_*
```

### Note: wal-g hangups

The backup command `wal-g backup-push <path>` hangs up after `pg_stop_backup()`
if a wal backup (triggered by postgresql server) has been failed.

I found the relevant log in `/var/log/postgresql/postgresql-10-main.log`:

```
2019-10-27 12:59:53.503 UTC [1466] postgres@postgres WARNING:  pg_stop_backup still waiting for all requi
red WAL segments to be archived (480 seconds elapsed)
2019-10-27 12:59:53.503 UTC [1466] postgres@postgres HINT:  Check that your archive_command is executing
properly.  pg_stop_backup can be canceled safely, but the database backup will not be usable without all
the WAL segments.
```

This was a configuration mistake: missing `%p` in the `archive_command`:

```
- archive_command = '/bin/uenv -f /srv/wal-g/minio.env /srv/wal-g/wal-g wal-push'
+ archive_command = '/bin/uenv -f /srv/wal-g/minio.env /srv/wal-g/wal-g wal-push "%p"'
```

This causes `wal-g wal-push` failure, which causes `pg_stop_backup` lockup and
which in turn causes external `wal-g backup-push` hangup.

This is only a single possible cause. `wal-push` may hang up for different
reasons. As seen in this closed-but-not-really-resolved issue:

- https://github.com/wal-g/wal-g/issues/124

So watch out in production!
