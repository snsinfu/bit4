class Loop:
    def __init__(self, n):
        self._n = n

    def __iter__(self):
        def generator():
            print("Start")
            for i in range(self._n):
                try:
                    yield i
                except GeneratorExit:
                    break
            print("Finish")
        return generator()


for i in Loop(5):
    print(i)

for i in Loop(5):
    print(i)
    break
