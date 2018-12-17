Service directory should contain `run` executable and `log` directory. The `log`
directory should also contain its own `run` executable. These two `run` scripts
are `supervise`d by daemontools.

    service/
      icecast/
        run*
        log/
          run*

The stdout of service `run` script is piped to the stdin of log `run` script.
Typically, log `run` script just executes multilog:

```sh
#!/bin/sh
exec multilog .
```

Note the `exec` instruction. The daemontools superviser uses the pid of `run`
script to control a service daemon. So the `run` script needs to exec a daemon
program, which is `multilog` here.
