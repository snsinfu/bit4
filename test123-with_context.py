class Context:
    def __enter__(self):
        print('enter')

    def __exit__(self, extype, ex, traceback):
        print('exit')

with Context() as ctx:
    pass
