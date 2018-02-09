import argparse
import itertools
import os
import signal
import sys


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    arg = parser.add_argument

    arg('-b', dest='bin_size', type=int, default=1000)
    arg('-s', dest='series_dat', type=argparse.FileType('r'))
    arg('src', type=argparse.FileType('r'), default=sys.stdin, nargs='?')

    return vars(parser.parse_args())


def run(src, bin_size, series_dat):
    series_sizes = load_series_sizes(series_dat)

    for name, series in stream_series(scan_source(src)):
        hard_end = series_sizes[name]
        for beg, end, value in stream_bins(series, bin_size, hard_end):
            print(f'{name}\t{beg}\t{end}\t{value:.6g}')


def load_series_sizes(series_dat):
    series_sizes = {}

    for line in series_dat:
        name, size = line.split(None, 1)
        series_sizes[name] = int(size)

    return series_sizes


def scan_source(source):
    for line in source:
        name, beg, end, value = line.split(None, 3)
        beg = int(beg)
        end = int(end)
        value = float(value)
        yield name, beg, end, value


def stream_series(source):
    for name, series in itertools.groupby(source, lambda record: record[0]):
        series = ((beg, end, value) for name, beg, end, value in series)
        yield name, series


class Bin:
    def __init__(self, beg=None, end=None, value=0.0):
        self.beg = beg
        self.end = end
        self.value = value


class BinStreamException(Exception):
    pass


def stream_bins(source, bin_size, hard_end):
    cur = Bin(0, min(bin_size, hard_end))

    def pop_bin():
        bin_beg = cur.beg
        bin_end = cur.end
        bin_value = cur.value / (cur.end - cur.beg)

        cur.beg += bin_size
        cur.end = min(cur.end + bin_size, hard_end)
        cur.value = 0.0

        return bin_beg, bin_end, bin_value

    for beg, end, value in source:
        if end <= beg:
            raise BinStreamException('input contains degenerate or reverse interval')

        if end <= cur.beg:
            raise BinStreamException('input is not sorted')

        if end > hard_end:
            raise BinStreamException('input overshoots')

        while cur.end <= beg:
            yield pop_bin()

        while cur.beg <= end - bin_size:
            cur.value += value * bin_size
            yield pop_bin()

        if cur.beg < end:
            cur.value += value * (end - cur.beg)

    while cur.beg < hard_end:
        yield pop_bin()


if __name__ == '__main__':
    main()
