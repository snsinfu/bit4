import argparse
import gzip
import io
import signal
import sys

import binner


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    run(**parse_args())


def parse_args():
    parser = argparse.ArgumentParser()
    arg = parser.add_argument

    arg('-b',
        '--bin',
        metavar='N',
        dest='bin_size',
        type=int,
        default=1000,
        help='Specify bin size [default: 1000]')

    arg('-s',
        '--series',
        metavar='FILE',
        dest='series_dat',
        type=argparse.FileType('r'),
        help='Specify optional series definition file')

    arg('-z',
        '--decompress',
        dest='decompress_src',
        action='store_true',
        default=False,
        help='Decompress input')

    arg('src',
        type=argparse.FileType('r'),
        nargs='?',
        default=sys.stdin,
        help='Specify input file [default: stdin]')

    return vars(parser.parse_args())


def run(src, decompress_src, bin_size, series_dat):
    if decompress_src:
        src = decompress_text(src)

    series = scan_source(src)
    series_sizes = load_series_sizes(series_dat) if series_dat else None

    bins = binner.bin_series(series, bin_size, series_sizes)
    row = '{:s}\t{}\t{}\t{:.6g}'

    for tag, beg, end, value in bins:
        print(row.format(tag, beg, end, value))


def decompress_text(src):
    src = src.detach()
    src = gzip.GzipFile(fileobj=src)
    src = io.TextIOWrapper(src)
    return src


def load_series_sizes(src):
    series_sizes = {}
    for line in src:
        tag, size = line.split(None, 1)
        series_sizes[tag] = int(size)
    return series_sizes


def scan_source(src):
    for line in src:
        tag, beg, end, value = line.split(None, 3)
        beg = int(beg)
        end = int(end)
        value = float(value)
        yield tag, beg, end, value
