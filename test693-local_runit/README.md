runit (and other daemontools workalikes) can supervise arbitrary program
without setting up centralized service directory hierarchy. Just create a
service directory containing a `run` script at arbitrary place.  Then:

```console
$ runsv service
```

The program is supervised and restarted on crash. Ctrl-C to terminate.
