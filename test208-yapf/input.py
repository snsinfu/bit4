import argparse
import signal
import sys

def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)
    run(**parse_args())

def run(src):
    prev = 0
    prev_delta = 0
    for num in map(int, src):
        delta = num - prev
        print(zigzag_encode(delta - prev_delta))
        prev_delta = delta
        prev = num

def zigzag_encode(num):
    num <<= 1
    if num < 0:
        num = ~num
    return num

def parse_args():
    parser = argparse.ArgumentParser(description='double-delta zig-zag encode integer sequence')
    parser.add_argument('src', type=argparse.FileType('r'), default=sys.stdin, nargs='?',
            help='input text file (default: stdin)')
    return vars(parser.parse_args())

if __name__ == '__main__':
    main()
