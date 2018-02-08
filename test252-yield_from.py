def main():
    for x in generate():
        print(x)


def generate():
    for step in range(10):
        yield from sub_generate(step)


def sub_generate(step):
    if step == 3:
        yield 'a'
        yield 'b'
    if step == 7:
        yield 'c'


if __name__ == '__main__':
    main()
