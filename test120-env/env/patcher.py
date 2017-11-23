import os
import types

class Patcher(types.ModuleType):
    def __init__(self, env, blacklist):
        for attr in dir(env):
            if attr not in blacklist:
                setattr(self, attr, getattr(env, attr))

    def __getattr__(self, key):
        return os.getenv(key)
