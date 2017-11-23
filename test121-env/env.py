import os

class Env(object):
    def __getattr__(self, key):
        return os.getenv(key)

    def __setattr__(self, key, value):
        os.environ[key] = value

    def __delattr__(self, key):
        del os.environ[key]

env = Env()
