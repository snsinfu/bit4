import sys

def main():
    print('Hello, python')

    for arg in sys.argv[1:]:
        print('Arg:', arg)

if __name__ == '__main__':
    main()
