wal-g backup to Google Cloud Storage. It works!

1. Create a project on Google Cloud console
2. Create a service account having storage privileges
3. Download credentials JSON and put it as `files/google.json`
4. Create a bucket and put `gs_bucket: the-name` in `host_vars/default`
5. Add the previously added service account as a privileged member of the bucket
6. `vagrant up`
7. Monitor the bucket (e.g., `gsutil du -sh gs://bucket/test`)

Looks like fresh DB consumes ~6 MiB per backup. So single-week retention would
consume at least 42 MiB of the storage. And the usage increases as the DB
grows, with the factor of 7.
