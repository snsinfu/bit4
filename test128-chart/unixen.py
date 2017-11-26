import errno
import functools
import os
import signal

def invoke_default_signal_handler(sig):
    signal.signal(sig, signal.SIG_DFL)
    os.kill(os.getpid(), sig)

def catch(func):
    @functools.wraps(func)
    def unwrap(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except KeyboardInterrupt:
            invoke_default_signal_handler(signal.SIGINT)
        except IOError as ex:
            if ex.errno == errno.EPIPE:
                invoke_default_signal_handler(signal.SIGPIPE)
            raise ex
    return unwrap
